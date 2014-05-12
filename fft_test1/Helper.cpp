namespace Helper {
    void printVectorFloat(const vector<float> &vecIn)
    {
        for(int i=0; i<vecIn.size(); i++)
            std::cout << vecIn[i] << "\n";
    }
    void printVectorInt(const vector<int> &vecIn)
    {
        for(int i=0; i<vecIn.size(); i++)
            std::cout << vecIn[i] << "\n";
    }
}