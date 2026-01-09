#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"

// 函数：获取当前目录下所有 .root 文件
std::vector<std::string> find_root_files(const std::string& path = ".") {
    std::vector<std::string> root_files;
    DIR* dir;
    struct dirent* ent;
    struct stat st;

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string file_name = ent->d_name;
            if (file_name == "." || file_name == "..") {
                continue;
            }

            std::string full_path = path + "/" + file_name;
            if (stat(full_path.c_str(), &st) == -1) {
                continue;
            }

            if ((st.st_mode & S_IFMT) == S_IFREG) {
                if (file_name.size() > 5 && 
                    file_name.substr(file_name.size() - 5) == ".root") {
                    root_files.push_back(full_path);
                }
            }
        }
        closedir(dir);
    } else {
        std::cerr << "错误: 无法打开目录 " << path << std::endl;
    }

    return root_files;
}

// 函数：统计单个 ROOT 文件中所有 TTree 的分支数量
void count_tree_branches(const std::string& root_file_path) {
    std::cout << "\n分析文件: " << root_file_path << std::endl;

    // 打开 ROOT 文件
    TFile* file = TFile::Open(root_file_path.c_str());
    if (!file || file->IsZombie()) {
        std::cerr << "  错误: 无法打开文件或文件损坏" << std::endl;
        return;
    }

    // 获取文件中的所有键
    TIter next(file->GetListOfKeys());
    TKey* key;
    bool found_tree = false;

    int TotalEntry = 0;

    while ((key = (TKey*)next())) {
        // 获取对象并检查是否是 TTree
        TObject* obj = key->ReadObj();
        if (obj->InheritsFrom("TTree")) {
            found_tree = true;
            TTree* tree = (TTree*)obj;
            int branch_count = tree->GetEntries();
            TotalEntry += branch_count;

            // std::cout << "  树 '" << tree->GetName() << "' 有 " 
            //           << branch_count << " 个分支" << std::endl;
        }
    }
    if (found_tree) {
        std::cout <<  "' 有 "
                  << TotalEntry << " 个事例" << std::endl;
    }

    if (!found_tree) {
        std::cout << "  警告: 文件中没有找到任何 TTree" << std::endl;
    }

    // 关闭文件
    file->Close();
    delete file;
}

int CountTreeNumber() {
    std::cout << "正在扫描当前目录下的 ROOT 文件..." << std::endl;
    
    // 获取当前目录下所有 .root 文件
    auto root_files = find_root_files();
    
    if (root_files.empty()) {
        std::cout << "没有找到任何 .root 文件" << std::endl;
        return 0;
    }

    std::cout << "找到 " << root_files.size() << " 个 ROOT 文件:" << std::endl;
    for (const auto& file : root_files) {
        std::cout << "  " << file << std::endl;
    }

    // 分析每个 ROOT 文件
    for (const auto& file : root_files) {
        count_tree_branches(file);
    }

    return 0;
}