struct Vector3D {
    double x,y,z;
};

double DotProduct(Vector3D A, Vector3D B)//求两个向量的点乘
{
    double result;
    result = A.x*B.x + A.y*B.y + A.z*B.z;
    return result;
}

double Norm(Vector3D A)//求向量的模
{
    double result;
    result = sqrt(pow(A.x, 2) + pow(A.y, 2) + pow(A.z, 2));
    return result;
}

Vector3D VectorMinus(Vector3D A,Vector3D B)//A向量减B向量
{
    Vector3D result;
    result.x = A.x - B.x;
    result.y = A.y - B.y;
    result.z = A.z - B.z;
    return result;
}

Vector3D VectorAdd(Vector3D A,Vector3D B)//A向量加B向量
{
    Vector3D result;
    result.x = A.x + B.x;
    result.y = A.y + B.y;
    result.z = A.z + B.z;
    return result;
}

Vector3D Multiply(double A,Vector3D B)//A乘B向量
{
    Vector3D result;
    result.x = A*B.x;
    result.y = A*B.y;
    result.z = A*B.z;
    return result;
}



void test() 
{
    Vector3D ImagePoint;
    Vector3D Edep_Position;
    Vector3D NodePosition;

    Edep_Position.x = 0;
    Edep_Position.y = 0;
    Edep_Position.z = 0;

    NodePosition.x = 0;
    NodePosition.y = 0;
    NodePosition.z = sqrt(17700*17700 - NodePosition.x*NodePosition.x - NodePosition.y*NodePosition.y);
    
    double RLS = 17700.;
    double RPMT = 19434.;
    double RfrIndxLS = 1.54;
    double RfrIndxWR = 1.355;

    Vector3D Input = VectorMinus(NodePosition,Edep_Position);
    Vector3D NormalLine;
    NormalLine.x = -NodePosition.x;
    NormalLine.y = -NodePosition.y;
    NormalLine.z = -NodePosition.z;

    Vector3D VA = VectorAdd(Multiply(-DotProduct(Input,NormalLine)/(Norm(NormalLine)*Norm(NormalLine)), NormalLine),Input);
    // cout<<"VA x="<<VA.x<<"; y="<<VA.y<<"; z="<<VA.z<<endl;
    double SinOut = RfrIndxLS*Norm(VA)/(RfrIndxWR*Norm(Input));
    // double CosIn = DotProduct(Input,NormalLine)/(Norm(Input)*Norm(NormalLine));
    // cout<<"SinIn = "<<sqrt(1-CosIn*CosIn)<<endl;
    // cout<<"SinOut ="<<SinOut<<endl;
    // cout<<"SinIn/SinOut = "<<sqrt(1-CosIn*CosIn)/SinOut<<" Ref="<<RfrIndxWR/RfrIndxLS<<endl;
    Vector3D Output = VectorMinus(Multiply(SinOut/Norm(VA),VA),Multiply(sqrt(1-SinOut*SinOut)/Norm(NormalLine),NormalLine));
    if (Norm(VA) == 0)
    {
        Output = VectorMinus(VA,Multiply(1./Norm(NormalLine),NormalLine));
    }
    
    // cout<<"Output x="<<Output.x<<"; y="<<Output.y<<"; z="<<Output.z<<endl;

    double LenthOfOutput = -RLS*sqrt(1-SinOut*SinOut) + sqrt(RLS*sqrt(1-SinOut*SinOut)*RLS*sqrt(1-SinOut*SinOut) - (RLS*RLS-RPMT*RPMT));

    ImagePoint = VectorAdd(NodePosition,Multiply(LenthOfOutput,Output));

    cout<<"x="<<ImagePoint.x<<"; y="<<ImagePoint.y<<"; z="<<ImagePoint.z<<endl;
    cout<<"R= "<<Norm(ImagePoint)<<endl;
}