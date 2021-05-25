/** spawner.hpp Author "sinhnn <sinhnn>" Date 15.03.2021  */

#ifndef SPAWNER_HPP_UTYWKCOQ
#define SPAWNER_HPP_UTYWKCOQ

#include "json/json.hpp"
#include "inja/inja.hpp"

#include <string>
#include <vector>
#include <QtGlobal>
#include <QString>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

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
vector<string> parse_execContextTemplate(vector<string> contextTemplate, const json& data) {
    vector<string> exectionArgs;
    for (vector<string>::const_iterator i = contextTemplate.begin(); i != contextTemplate.end(); ++i) {
        try {
            exectionArgs.push_back(inja::render(*i, data));
        } catch(const std::exception& e) {
            QMessageBox msgBox;
            msgBox.setText("Unable to parse_execContextTemplate!");
            msgBox.setInformativeText(e.what());
            msgBox.exec();
            return vector<string>();
        } catch (const std::runtime_error& e) {
            std::cout << " a standard exception was caught, with message '" << e.what() << "'\n";
            // this executes if f() throws std::underflow_error (base class rule)
        } catch (const std::overflow_error& e) {
            std::cout << " a standard exception was caught, with message '" << e.what() << "'\n";
            // this executes if f() throws std::overflow_error (same type rule)
            return vector<string>();
        } catch (...) {
            QMessageBox msgBox;
            msgBox.setText("Unable to parse_execContextTemplate!");
            msgBox.exec();
            return vector<string>();
        }
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
int exec_execContextTemplate(vector<string> contextTemplate, const json& data) {
    if (contextTemplate.empty()) return ERROR_CODE_NO_EXECCONTEXT;
    qint64 pid = -1;
    QProcess proc;
    /* execContext and its feedback to sqlite db */
    vector <string> realArgs = parse_execContextTemplate(contextTemplate, data);
#ifdef DEBUG
    for (int i = 0; i < realArgs.size(); ++i) {
        qDebug() << "realArgs.at(" << i << ") = " << realArgs.at(i);
    }
#endif
    if (realArgs.empty()) {
        return pid;
    }
    QStringList arguments;
    for (int i = 1; i < realArgs.size(); i++) {
      if (realArgs.at(i).empty()) continue;
      arguments << QString::fromStdString(realArgs.at(i));
#ifdef DEBUG
      qDebug() << arguments.at(i).toStdString() << std::endl;
#endif
    }
    QString program = QString::fromStdString(realArgs.at(0));

    try {
        proc.startDetached(program, arguments, "", &pid);
    } catch(...) {
        QMessageBox msgBox;
        msgBox.setText("Unable to start your executation context!");
        msgBox.setInformativeText(program + QString(" ") + arguments.join(" "));
        msgBox.exec();
    }
    return pid;
}

#endif /* end of include guard: SPAWNER_HPP_UTYWKCOQ */
