#include <iostream>

using namespace std;

class Girl;

class Boy
{
public:
    Boy() { cout << "Boy construct ..." << endl; }
    ~Boy() { cout << "Boy destroy ..." << endl; }
    void setGirlFriend(shared_ptr<Girl> girlFriend)
    {
        m_girlFriend = girlFriend;
        shared_ptr<Girl> spgirl;
        spgirl = m_girlFriend.lock(); //获取监视的shared_ptr
        cout << __FUNCTION__ << " spgirl " << spgirl.use_count() << endl;
    }

private:
    //    shared_ptr<Girl> m_girlFriend;
    weak_ptr<Girl> m_girlFriend;
};

class Girl
{
public:
    Girl() { cout << "Girl construct ..." << endl; }
    ~Girl() { cout << "Girl destroy ..." << endl; }
    void setBoyFriend(shared_ptr<Boy> boyFriend) { m_boyFriend = boyFriend; }

private:
    shared_ptr<Boy> m_boyFriend;
};

/*
int main()
{
    shared_ptr<Boy> spboy(new Boy);
    shared_ptr<Girl> spGirl(new Girl);

    spboy->setGirlFriend(spGirl);
    spGirl->setBoyFriend(spboy); //不使用weak_ptr内存泄漏

    return 0;
}*/
