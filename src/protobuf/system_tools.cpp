// system_tools.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "system_tools.h"
#include <fstream>
#include <google/protobuf/text_format.h>
#include "tasmet_types.h"
#include "tasmet_io.h"
#include "tasmet_exception.h"

#include <google/protobuf/text_format.h>

using google::protobuf::TextFormat;

pb::System loadSystem(const string& filepath) {

    TRACE(15,"loadSystem()");
    VARTRACE(15,filepath);
    std::ifstream myfile(filepath,ios::binary);
        
    if(!myfile.good()) {
        string error = "Read error on ";
        error += filepath;
        throw TaSMETError(error);
        
    }

    pb::System sys;

    std::stringstream strStream;
    strStream << myfile.rdbuf(); //read the file
    string data = strStream.str();//str holds the content of the file        
        
    if(!TextFormat::ParseFromString(data,&sys)) {
        string error = "Invalid TaSMET Model file: ";
        error += filepath;
        throw TaSMETError(error);
    }
        
    return sys;


}
void saveSystem(const string& filepath,const pb::System& sys) {

    std::ofstream sfile(filepath,ios::binary);
    if(!sfile.good()){
        throw TaSMETError("Could not write to file");
    }
    string data;
    if(TextFormat::PrintToString(sys,&data)) {
        // Can maybe assign to itself. Which is no problem
        // according to C++ docs
        TRACE(15,"Saving file succeeded");
        sfile << data << std::flush;
        
        // Close file here, such that in can be opened to compare
        // whether the file is still dirty 
        sfile.close();
    }
    else {
        throw TaSMETError("Could not save model to file");
        
    }
}
// // Returns true when the two systems are equal
bool compareSys(const pb::System& s1,const pb::System& s2) {
    return (s1.SerializeAsString()==s2.SerializeAsString());
}

//////////////////////////////////////////////////////////////////////

