#pragma once

template<typename T>
class singleton
{
public:
    static T& GetInstance()
    {
        static T theSingleInstance; // � ������ T ���� ����������� �� ���������
        return theSingleInstance;
    }
};
