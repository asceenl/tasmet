// tasmet_exception.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Implementation of exception TasMETBadAlloc
//////////////////////////////////////////////////////////////////////

#include "tasmet_exception.h"
#include <QString>

const char* TaSMETError::what() const throw() {
    return _msg.c_str();
}
void TaSMETError::setContext(const std::string& ctx) {
    std::string oldmsg = _msg;
    _msg = ctx + ": " + oldmsg;
}
const char*  TaSMETBadAlloc::what() const throw() {
	return "Error: memory allocation failed. "
   		"Please make sure enough memory is available and restart the application";
}
void TaSMETError::show_user(const std::string& window_title,
                            QMessageBox::Icon icon) {

    QString msg = what();
    QMessageBox msgbx(icon,
                    QString::fromStdString(window_title),
                    msg);

    msgbx.exec();

}

//////////////////////////////////////////////////////////////////////

