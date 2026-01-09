void test()
{
    for (int i = 1; i < 10; i++)//均值为“xx”时，测到i PE且过阈的概率。认为“xx”几乎不会超过5PE
    {
        cout<<ROOT::Math::normal_cdf(1.0/3.0,(33.2/100.0)*sqrt((double)i),(double)i)<<endl;
    }
}