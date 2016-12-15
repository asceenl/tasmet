// globalconf.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "globalconf.h"
#include "tasmet_constants.h"
#include "tasmet_exception.h"
#include "tasmet_io.h"

GlobalConf::GlobalConf(us Nf,d freq):
    _Nf(Nf)
{

    TRACE(10,"GlobalConf constructor done");

    if(Nf>=constants::maxNf)
        throw TaSMETError("Too large number of frequencies given");


    us Ns=this->Ns();
    
    // Reinitialize all operators
    _iDFT=zeros<dmat>(Ns,Ns);
    _fDFT=zeros<dmat>(Ns,Ns);

    _fDFT.row(0).fill(1.0/double(Ns));

    for(us i=1;i<=_Nf;i++){
        for(us j=0; j<Ns;j++){
            //Row i+1 (cosine components)
            _fDFT(2*i-1,j)=2.0*cos(2.0*number_pi*double(i)*double(j)/double(Ns))/double(Ns);
            //Row i (sine components)
            _fDFT(2*i,j)=-2.0*sin(2.0*number_pi*double(i)*double(j)/double(Ns))/double(Ns);
        }
    }

    _iDFT.col(0).fill(1.0);	// Steady part
    for(us k=0;k<Ns;k++){
        for (us n=1;n<=_Nf;n++){
            _iDFT(k,2*n-1)=cos(2.0*number_pi*double(n)*double(k)/Ns);
            _iDFT(k,2*n)=-sin(2.0*number_pi*double(n)*double(k)/Ns);
        }
    }

    setfreq(freq);

}
void GlobalConf::show() const {
    cout << "------- Global configuration ------ \n";        
    cout << "------- Number of harmonics to solve for: "<< _Nf <<"\n";
    cout << "------- Fundamental frequency           : " << _omg/2/number_pi << " Hz\n";           
}
void GlobalConf::setomg(d omg){
    TRACE(15,"GlobalConf::set(_Nf,freq)");


    _DDTfd=zeros<dmat>(Ns(),Ns());

    // Sanity checks
    if(omg<constants::minomg && omg>constants::maxomg)
        throw TaSMETError("Illegal frequency given");

    this->_omg=omg;

    for(us i=1;i<=_Nf;i++){
        _DDTfd(2*i-1,2*i  )=-double(i)*_omg;
        _DDTfd(2*i  ,2*i-1)=double(i)*_omg;
    }

}

//////////////////////////////////////////////////////////////////////

