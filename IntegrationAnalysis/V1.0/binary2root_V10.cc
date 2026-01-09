// 编译命令：g++ binary2root_V10.cc -o bin2root `root-config --cflags --libs`
#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <TString.h>
using namespace std;

// 1. 定义截图对应的结构体（匹配27B Event Header + 9B Channel Data）
struct EventHeader {
    uint32_t code_header;       // 4B: 帧头魔数（如0xBB1234BB）
    uint32_t abc_timestamp;     // 28b: ABC时间戳（打包在5B数据中）
    uint16_t temp_adc;          // 12b: 温度ADC原始值（打包在5B数据中）
    uint8_t num_recorded_ch;    // 1B: 记录的通道数量
    uint64_t hit_ch_high;       // 8B: 通道命中标记[127~64]（1bit/通道）
    uint8_t padding;            // 1B: 填充字节（无用）
    uint64_t hit_ch_low;        // 8B: 通道命中标记[63~0]（1bit/通道）
};

struct ChannelData {
    uint8_t gain_ch;            // 1B: 增益(1bit) + 通道号(7bit)
    uint32_t coarse_time;       // 4B: 粗时间戳
    uint16_t charge;            // 2B: 电荷值
    uint16_t fine_time;         // 2B: 细时间戳
};

// 2. 解析工具函数：从5B打包数据中提取28b ABC时间戳和12b温度ADC
void parseABCAndTemp(const char* data, uint32_t& abc_ts, uint16_t& temp_adc) {
    uint64_t combined = 0;
    // 大端序合并5B数据（data[0]为最高位字节）
    for (int i = 0; i < 5; ++i) {
        combined |= (static_cast<uint64_t>(data[i] & 0xFFu)) << ((4 - i) * 8);
    }
    abc_ts = static_cast<uint32_t>((combined >> 12) & 0x0FFFFFFFu); // 高28位（28bit全1掩码0x0FFFFFFF）
    temp_adc = static_cast<uint16_t>(combined & 0x00000FFFu);       // 低12位（12bit全1掩码0x0FFF）
}

// 解析工具函数：从1B gain_ch中拆分增益和通道号（bit7=增益，bit6~0=通道号）
void parseGainAndChannel(uint8_t gain_ch, uint32_t& gain, uint32_t& ch_num) {
    gain = (gain_ch >> 7) & 0x01u;  // 最高位：1=高增益(HG)，0=低增益(LG)
    ch_num = gain_ch & 0x7Fu;       // 低7位：通道号（0~127，匹配128通道设计）
}

void parseSignAndCharge(const char* data, uint32_t& sign, uint32_t& charge) {
	uint64_t combined = 0;
    // 大端序合并2B数据（data[0]为最高位字节）
    for (int i = 0; i < 2; ++i) {
        combined |= (static_cast<uint64_t>(data[i] & 0xFFu)) << ((1 - i) * 8);
    }
	// sign为第11位
	sign = (combined >> 15) & 0x00000001u; 
	charge = static_cast<uint16_t>(combined & 0x000003FFu);      
}

// 解析工具函数：大端序合并N字节数据为uint64_t（通用）
template <typename T>
T mergeBytesBigEndian(const char* data, int byte_len) {
    T result = 0;
    for (int i = 0; i < byte_len; ++i) {
        result |= (static_cast<T>(data[i] & 0xFFu)) << ((byte_len - 1 - i) * 8);
    }
    return result;
}

int main(int argc, char **argv) {
    // 3. 命令行参数检查（输入二进制文件 + 输出ROOT文件）
    if (argc != 3) {
        cerr << "用法：./bin2root [输入二进制文件路径] [输出ROOT文件路径]" << endl;
        return 1;
    }
    string in_path(argv[1]);
    TString out_path(argv[2]);

    // 4. 打开输入二进制文件
    ifstream input_file(in_path, ios::binary);
    if (!input_file.is_open()) {
        cerr << "错误：无法打开输入文件 " << in_path << endl;
        return 1;
    }

    // 5. 创建输出ROOT文件
    TFile output_file(out_path, "RECREATE");
    if (output_file.IsZombie()) {
        cerr << "错误：无法创建输出ROOT文件 " << out_path << endl;
        input_file.close();
        return 1;
    }

    // 6. 定义缓冲区（避免动态内存泄漏，比char*更安全）
    vector<char> magic_buf(4);        // 4B：用于查找帧头魔数
    vector<char> evt_header_buf(27);  // 27B：Event Header缓冲区
    vector<char> ch_data_buf(9);      // 9B：Channel Data缓冲区

    // 7. 查找Event Header帧头魔数（4B，0xCAFECAFE）
    bool found_magic = false;
    while (input_file.read(magic_buf.data(), magic_buf.size())) {
        uint32_t magic = mergeBytesBigEndian<uint32_t>(magic_buf.data(), 4);
        if (magic == 0xCAFECAFE) {
            cout << "找到帧头魔数：0xCAFECAFE" << endl;
            input_file.seekg(-4, ios::cur); // 回退4B，后续读取完整27B Header
            found_magic = true;
            break;
        }
    }
    if (!found_magic) {
        cerr << "错误：未在输入文件中找到帧头魔数0xCAFECAFE" << endl;
        input_file.close();
        return 1;
    }

    // 8. 定义ROOT Tree及关联变量（匹配截图字段）
    TTree event_tree("EventData", "Data Tree");
    uint32_t event_idx = 0;          // 事件索引（标记第几个Event Header）
    uint32_t code_header;            // 帧头魔数
    uint32_t abc_timestamp;          // ABC时间戳（28b）
    uint16_t temp_adc;               // 温度ADC原始值（12b）
    double temp_celsius;             // 计算后的温度（°C，截图公式）
    uint8_t num_recorded_ch;         // 记录的通道数
    uint64_t hit_ch_high;            // 通道命中[127~64]
    uint64_t hit_ch_low;             // 通道命中[63~0]
    uint32_t gain;                    // 增益（0=LG，1=HG）
    uint32_t ch_num;                  // 通道号（0~127）
    uint32_t coarse_time;            // 粗时间戳（4B）
    uint32_t charge;                 // 电荷值（4B）
    uint32_t fine_time;              // 细时间戳（4B）
	uint32_t sign;

    // 关联Tree分支（类型匹配：i=int, s=short, b=byte, l=long long, D=double）
    event_tree.Branch("EventIndex", &event_idx, "EventIndex/i");
    // event_tree.Branch("CodeHeader", &code_header, "CodeHeader/i");
    event_tree.Branch("ABCTimestamp", &abc_timestamp, "abcTimestamp/i");
    // event_tree.Branch("TempADC", &temp_adc, "TempADC/s");
    // event_tree.Branch("TemperatureCelsius", &temp_celsius, "TemperatureCelsius/D");
    // event_tree.Branch("NumRecordedChannels", &num_recorded_ch, "NumRecordedChannels/b");
    // event_tree.Branch("HitChannelHigh", &hit_ch_high, "HitChannelHigh/l");
    // event_tree.Branch("HitChannelLow", &hit_ch_low, "HitChannelLow/l");
    event_tree.Branch("Gain", &gain, "gain/i");
	event_tree.Branch("Sign", &sign, "sign/i");
    event_tree.Branch("ABCChannelNumber", &ch_num, "channelNumber/i");
    event_tree.Branch("CoarseTime", &coarse_time, "coarseTime/i");
    event_tree.Branch("Charge", &charge, "chargeValue/i");
    event_tree.Branch("FineTime", &fine_time, "fineTime/i");

    // 9. 循环读取Event Header和Channel Data
    while (input_file.read(evt_header_buf.data(), evt_header_buf.size())) {
        event_idx++;

        // 9.1 解析27B Event Header（按截图字段偏移）
        code_header = mergeBytesBigEndian<uint32_t>(evt_header_buf.data() + 0, 4); // 0~3B
		if (code_header != 0xCAFECAFE)
		{
			cout << "错误：事件" << event_idx << "的帧头魔数不是0xCAFECAFE" << endl;
			continue;
		}
		
        parseABCAndTemp(evt_header_buf.data() + 4, abc_timestamp, temp_adc);       // 4~8B（5B打包）
        temp_celsius = static_cast<double>(temp_adc) * 503.975 / 4096 - 273.15;   // 温度计算（截图公式）
        num_recorded_ch = static_cast<uint8_t>(evt_header_buf[9]);                // 9B
        hit_ch_high = mergeBytesBigEndian<uint64_t>(evt_header_buf.data() + 10, 8); // 10~17B
        hit_ch_low = mergeBytesBigEndian<uint64_t>(evt_header_buf.data() + 19, 8);  // 19~26B（跳过18B padding）

        // 调试信息（可选，确认解析正确性）
        // cout << "事件" << event_idx << "："
        //      << "ABC时间戳=0x" << hex << abc_timestamp << dec
        //      << "，温度=" << temp_celsius << "°C"
        //      << "，通道数=" << static_cast<int>(num_recorded_ch) << endl;

        // 9.2 解析当前Event Header对应的N个Channel Data（每个9B）
        while (input_file.read(ch_data_buf.data(), ch_data_buf.size())) {
            // 读取9B通道数据（若数据不完整则跳出）
            // if (!input_file.read(ch_data_buf.data(), ch_data_buf.size())) {
            //     cerr << "警告：事件" << event_idx << "的通道" << static_cast<int>(ch_idx) << "数据不完整" << endl;
            //     break;
            // }

			// 判断如果检测到魔数0xCAFECAFE，则回退且跳过
            if (mergeBytesBigEndian<uint32_t>(ch_data_buf.data(), 4) == 0xCAFECAFE) {
                input_file.seekg(-9, ios::cur); // 回退9B
                // cerr << "警告：事件" << event_idx << "的通道" << static_cast<int>(ch_idx) << "数据包含魔数0xCAFECAFE，跳过" << endl;
                break;
            }

            // 解析9B Channel Data（按截图字段偏移）
            uint8_t gain_ch = static_cast<uint8_t>(ch_data_buf[0]); // 0B：gain+channel
            parseGainAndChannel(gain_ch, gain, ch_num);
            coarse_time = mergeBytesBigEndian<uint32_t>(ch_data_buf.data() + 1, 4); // 1~4B
			parseSignAndCharge(ch_data_buf.data() + 5, sign, charge);
            fine_time = mergeBytesBigEndian<uint16_t>(ch_data_buf.data() + 7, 2);    // 7~8B

            // 填充Tree并写入
            event_tree.Fill();
        }
    }

    // 10. 写入Tree并关闭文件
    event_tree.Write();
    output_file.Close();
    input_file.close();
    cout << "成功：数据已转换为ROOT文件 " << out_path << endl;

    return 0;
}