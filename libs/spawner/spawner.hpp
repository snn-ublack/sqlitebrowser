/** spawner.hpp Author "sinhnn <sinhnn>" Date 15.03.2021  */

#ifndef SPAWNER_HPP_UTYWKCOQ
#define SPAWNER_HPP_UTYWKCOQ

#include "json/json.hpp"
#include "inja/inja.hpp"

#include <QtGlobal>
#include <QString>
#include <QProcess>
#include <QMessageBox>
// #include <glog/logging.h>

using json = nlohmann::json;
using namespace std;

#define ERROR_CODE_NO_EXECCONTEXT -1
/**
* @brief: convert inja template arguments to string
*
* @param: std::vecor<std::string> contextTemplate
*
* @return: std::vector<std::string>
*/
vector<string> parse_execContextTemplate(vector<string> contextTemplate, const json& data)
{
    vector<string> exectionArgs;
    for (vector<string>::const_iterator i = contextTemplate.begin(); i != contextTemplate.end(); ++i) {
        exectionArgs.push_back(inja::render(*i, data));
    }
    return exectionArgs;
}

/**
* @brief: brief
*
* @param: vector<string> contextTemplate
*       : const json& data
*
* @return: int pid of process
*/
int exec_execContextTemplate(vector<string> contextTemplate, const json& data)
{
    if (contextTemplate.size() <= 0) return ERROR_CODE_NO_EXECCONTEXT;
    qint64 pid = -1;
    QProcess proc;
    /* execContext and its feedback to sqlite db */
    vector <string> realArgs = parse_execContextTemplate(contextTemplate, data);
    // for (int i = 0; i < realArgs.size(); ++i) {
    //     LOG(INFO) << "realArgs.at(" << i << ") = " << realArgs.at(i);
    // }
    QStringList arguments;
    for (int i = 1; i < realArgs.size(); i++) {
      if (realArgs.at(i).empty()) continue;
      arguments << QString::fromStdString(realArgs.at(i));
    }
    QString program = QString::fromStdString(realArgs.at(0));
    try {
        proc.startDetached(program, arguments, "", &pid);
    }catch(...) {
        QMessageBox msgBox;
        msgBox.setText("Unable to start your executation context!");
        msgBox.setInformativeText(program + QString(" ") + arguments.join(" "));
        msgBox.exec();
    }
    return pid;
}

#endif /* end of include guard: SPAWNER_HPP_UTYWKCOQ */
