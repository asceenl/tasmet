// message_tools.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "message_tools.h"
#include <fstream>
#include "tasmet_types.h"
#include "tasmet_io.h"
#include "tasmet_exception.h"
#include "model.pb.h"

#include <google/protobuf/util/message_differencer.h>
// #include <google/protobuf/text_format.h>
// using google::protobuf::TextFormat;

template<typename T>
T loadMessage(const string& filepath) {

    TRACE(15,"loadMessage()");
    VARTRACE(15,filepath);
    std::ifstream myfile(filepath,ios::binary);
        
    if(!myfile.good()) {
        string error = "Read error on ";
        error += filepath;
        throw TaSMETError(error);
        
    }

    T sys;

    // std::stringstream strStream;
    // strStream << myfile.rdbuf(); //read the file
    // string data = strStream.str();//str holds the content of the file        
        
    if(!sys.ParseFromIstream(&myfile)) {
        string error = "Invalid TaSMET Model file: ";
        error += filepath;
        throw TaSMETError(error);
    }
        
    return sys;


}
template<typename T>
void saveMessage(const string& filepath,const T& sys) {

    std::ofstream sfile(filepath,ios::binary);
    if(!sfile.good()){
        throw TaSMETError("Could not write to file");
    }

    // if(TextFormat::PrintToString(sys,&data)) {
    if(sys.SerializeToOstream(&sfile)) {
        TRACE(15,"Saving file succeeded");
    }
    else {
        throw TaSMETError("Could not save model to file");
    }
}

// // Returns true when the two messages are equal
template<typename T>
bool compareMessage(const T& s1,const T& s2) {
    return google::protobuf::util::MessageDifferencer::Equals(s1,s2);
}

// Explicit instantiation for pb::Model
template
bool compareMessage<pb::Model>(const pb::Model& s1,const pb::Model& s2);

template
pb::Model loadMessage(const string& filepath);

template
void saveMessage<pb::Model>(const string& filepath,const pb::Model& model);
//////////////////////////////////////////////////////////////////////

