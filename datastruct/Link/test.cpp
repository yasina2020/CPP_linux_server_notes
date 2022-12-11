#include "m_link.hpp"
#include <string>
#include <iostream>

using namespace m_link;

typedef class STU{
public:
    STU(int m_id,std::string m_name):ID(m_id),name(m_name){}
    int ID;
    std::string name;
}stu;




void myPrint(void* arg){
    stu* student = (stu*)arg;
    std::cout<<"ID:"<<student->ID<<" name:"<<student->name<<"--";
}

int main(){
    m_link::Link *l = new m_link::Link;
    std::cout<<"l.size = "<<l->getLength()<<std::endl;
        stu* s1 = new stu(100,"张三");
        l->insterHead(s1);
        l->printLink(myPrint);
        std::cout<<std::endl;
        stu* s2 = new stu(101,"李四");
        l->insterHead(s2);
        l->printLink(myPrint);
        std::cout<<std::endl;
        stu* s3 = new stu(103,"wamhgwk");
        l->insterIndex(s3,2);
        l->printLink(myPrint);
        std::cout<<std::endl;
}