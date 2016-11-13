#include "globalconf.h"
#include "tasmet_constants.h"
#include "tasmet_exception.h"
#include "tasmet_io.h"

GlobalConf::GlobalConf(us Nf,d freq)

{

    set(Nf,freq);
    TRACE(10,"GlobalConf constructor done");
}
void GlobalConf::show() const {
    cout << "------- Global configuration ------ \n";        
    cout << "------- Number of harmonics to solve for: "<< _Nf <<"\n";
    cout << "------- Fundamental frequency           : " << _omg/2/number_pi << " Hz\n";           
}
void GlobalConf::set(us Nf,d freq){
    TRACE(15,"GlobalConf::set(_Nf,freq)");
    d omg = 2*number_pi*freq;
    //ctor
    // Sanity checks
    if(omg<constants::minomg && omg>constants::maxomg)
        throw TaSMETError("Illegal frequency given");
    if(Nf>=constants::maxNf)
        throw TaSMETError("Too large number of frequencies given");

    this->_Nf=Nf;
    this->_omg=omg;

    us Ns=this->Ns();
    // Reinitialize all operators
    iDFT_=zeros<dmat>(Ns,Ns);
    fDFT_=zeros<dmat>(Ns,Ns);

    DDTfd_=zeros<dmat>(Ns,Ns);
    fDFT_.row(0).fill(1.0/double(Ns));

    for(us i=1;i<=_Nf;i++){
        for(us j=0; j<Ns;j++){
            //Row i+1 (cosine components)
            fDFT_(2*i-1,j)=2.0*cos(2.0*number_pi*double(i)*double(j)/double(Ns))/double(Ns);
            //Row i (sine components)
            fDFT_(2*i,j)=-2.0*sin(2.0*number_pi*double(i)*double(j)/double(Ns))/double(Ns);
        }
    }

    iDFT_.col(0).fill(1.0);	// Steady part
    for(us k=0;k<Ns;k++){
        for (us n=1;n<=_Nf;n++){
            iDFT_(k,2*n-1)=cos(2.0*number_pi*double(n)*double(k)/Ns);
            iDFT_(k,2*n)=-sin(2.0*number_pi*double(n)*double(k)/Ns);
        }
    }

    for(us i=1;i<=_Nf;i++){
        DDTfd_(2*i-1,2*i  )=-double(i)*_omg;
        DDTfd_(2*i  ,2*i-1)=double(i)*_omg;
    }

}


