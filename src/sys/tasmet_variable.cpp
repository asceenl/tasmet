// var.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "tasmet_variable.h"
#include "tasmet_exception.h"
#define Ns (_gc->Ns())
#define Nf (_gc->Nf())
#define fDFT (_gc->fDFT)
#define iDFT (_gc->iDFT)
#define DDTfd (_gc->DDTfd)

//******************************************************************************** Operators
  
Variable operator*(const double& scalar,const Variable& other){ // Pre-multiplication with scalar
    TRACE(0,"Variable::operator*(scalar,Variable)");
    return Variable(other._gc,scalar*other._adata);
}
Variable Variable::operator/(const Variable& Variable2) const {
    TRACE(0,"Variable::operator/()");
    return Variable(this->_gc,this->_tdata/Variable2._tdata,false);
}
Variable Variable::operator/(d val) const {
    TRACE(0,"Variable::operator/(double)");
    return Variable(this->_gc,this->tdata()/val,false);
}
Variable Variable::operator-(const Variable& other) const{
    TRACE(0,"Variable::operator-(Variable)");
    return Variable(this->_gc,_adata-other._adata);
}
Variable Variable::operator*(d scalar) const {
    TRACE(0,"Variable::operator*(scalar)");	// Post-multiplication with scalar
    return Variable(this->_gc,scalar*_adata);
}
Variable Variable::operator+(const Variable& other) const{
    TRACE(0,"Variable::operator+()");
    return Variable(this->_gc,_adata+other._adata);
}
Variable Variable::operator*(const Variable& Variable2) const { // Multiply two
    TRACE(0,"Variable::operator*(const Variable& Variable2) const");
    return Variable(this->_gc,_tdata%Variable2._tdata,false);
}
//***************************************** The Variable class
Variable::Variable(const gc_ptr& gc): Variable(gc,0){}
Variable::Variable(const gc_ptr& gc,double initval): _gc(gc)
{
    TRACE(0,"Variable::Variable(us ndofs, double initval)");

    _tdata=initval*ones(Ns);
    _adata=zeros(Ns);
    _adata(0)=initval;
}
Variable::Variable(const gc_ptr& gc,const vd& data,bool adata): _gc(gc)
{ // Create a tasystem and fill it with time data.
    TRACE(0,"Variable::Variable(gc,_tdata)");
    if(adata){
        this->_adata=data;
        _tdata=iDFT*_adata;
    }
    else{
        this->_tdata=data;
        _adata=fDFT*_tdata;
    }
}
Variable::Variable(const gc_ptr& gc,const vc& data)
    :Variable(gc)
{ // Create a tasystem and fill it with time data.
    if(data.size()!=Nf+1)
        throw TaSMETError("Wrong size of amplitude vector given. Does the"
                      "vector size correspond to Ns?");
    setadata(data);
}
void Variable::setGc(const gc_ptr& gc){
    TRACE(10,"Variable::setGc()");
    this->_gc = gc;
    updateNf();
}
void Variable::resetHarmonics(){
    TRACE(10,"Variable::resetHarmonics()");
    if(Nf>1){
        for(us i=3;i<Ns;i++)
            _adata(i)=0;
    }
    _tdata=iDFT*_adata;
}
Variable::Variable(const Variable& other){
    TRACE(0,"Variable::Variable(const Variable& other)");
    this->_gc=other._gc;
    this->_tdata=other._tdata;
    this->_adata=other._adata;
}
Variable& Variable::operator=(const Variable& other){
    // THIS WOULD COUPLE TO THE WRONG GLOBALCONF when setRes is used
    // between ducts!!!!!
    if(this!=&other){
        if(!this->_gc) {
            this->_gc=other._gc;
        }
        this->_tdata=other._tdata;
        this->_adata=other._adata;
        updateNf();
    }
    return *this;
}
void Variable::updateNf(){
    TRACE(0,"Variable::updateNf()");
    us oldsize=_adata.size();
    assert(oldsize==_tdata.size());
    assert(_gc);
    us newsize=Ns;
    if(oldsize!=newsize){
        _adata.resize(newsize);
        _tdata=iDFT*_adata;
    }
}
// Get methods (which require implementation)
d Variable::operator()(us i) const {//Extract result at specific frequency
    TRACE(-2,"Variable::operator()("<<i<<"), Ns: "<< Ns);
    if(i>=Ns)
        throw TaSMETError("Invalid frequency number!");
    TRACE(-1,"_adata: "<<_adata);
    return _adata(i);
}  
vc Variable::getcRes() const
{
    TRACE(-2,"Variable::getcRes()");
    //	TRACE(0,"_adata:" << _adata);
    vc cadata(Nf+1);
    cadata(0)=_adata(0);
    for(us i=1;i<Nf+1;i++)
        cadata(i)=_adata(2*i-1)+I*_adata(2*i); //The minus is very important
    //	TRACE(0,"Resulting cadata:" << cadata);
    return cadata;
}
// Set methods
void Variable::setadata(us freqnr,d val) { //Set result for specific frequency zero,real one, -imag one, etc
    TRACE(-2,"Variable::setadata("<<freqnr<<","<<val<<")");
    if(freqnr>=Ns)
        throw TaSMETError("Tried to change a value beyond length of the vector");
    _adata[freqnr]=val;
    _tdata=iDFT*_adata;
}
void Variable::setadata(const vc& res)
{
    TRACE(0,"Variable::setadata(const vc& res)");
    assert(res.size()==Nf+1);
    _adata(0)=res(0).real();
    for(us i=1;i<Nf+1;i++){
        _adata(2*i-1)=res(i).real();
        _adata(2*i)=res(i).imag();
    }
    _tdata=iDFT*_adata;
}
d min(us a,us b){return a<=b?a:b;}
void Variable::setadata(const vd& val) {
    TRACE(0,"Variable::setadata(const vd& val)");
    _adata.zeros();
    us minsize=min(val.size(),_adata.size());
    for(us i=0;i<minsize;i++)
        _adata(i)=val(i);
    _tdata=iDFT*_adata;
}
void Variable::settdata(double val) {
    TRACE(0,"Variable::settdata(double val)");
    for (auto& td: _tdata) {
        td=val;
    }
    _adata=fDFT*_tdata;
}
void Variable::settdata(const vd& val) {
    TRACE(0,"Variable::settdata(vd& val)");
    assert(val.size()==_tdata.size());
    _tdata=val;
    _adata=fDFT*_tdata;
}

vd Variable::timeResponse(us nperiod,us ninst) const {
    TRACE(15,"vd Variable::timeResponse()");
    vd t=timeResponseTime(nperiod,ninst);
    vc cres=getcRes();
    vd res(t.size(),fillwith::zeros);
    c omg=_gc->getomg();
    for(us i=0;i<Nf+1;i++)
        res+=real(cres(i)*exp(((d) i)*omg*I*t));
    return res;
}
vd Variable::timeResponseTime(us nperiod,us ninst) const{
    d T=1/_gc->getfreq();
    return linspace(0,nperiod*T,ninst);
}
dmat Variable::freqMultiplyMat() const{
    TRACE(0,"Variable::freqMultiplyMat()");
    dmat result(Ns,Ns,fillwith::zeros);
    result(0,0)=_adata(0);
    if(Nf>0){
        for(us j=1;j<Nf+1;j++){
            result(2*j-1,2*j-1)= _adata(2*j-1);
            result(2*j-1,2*j  )=-_adata(2*j  ); // Yes only one
            // minus sign
            result(2*j  ,2*j-1)= _adata(2*j);      
            result(2*j  ,2*j  )= _adata(2*j-1);      
        }	// end for loop
    }	// if(Nf>0)
    return result;
}

//Get a tasystem which is the time derivative of the current one
Variable Variable::ddt() const {

    vd newadata=DDTfd*_adata;

    return Variable(_gc,newadata);
}
// The product

//***************************************** End of the Variable class







