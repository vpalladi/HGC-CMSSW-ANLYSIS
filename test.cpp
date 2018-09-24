
#include "iostream"
#include "TDatabasePDG.h"



int main(int argc, char* argv[]){

    std::cout << TDatabasePDG::Instance()->GetParticle(11)->GetName() << std::endl;
    return 0;

}
