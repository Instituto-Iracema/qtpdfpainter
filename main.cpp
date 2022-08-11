#include <QCoreApplication>
#include <QDir>
#include <QVariantMap>
#include <QVariantList>
#include <QDebug>
#include "pdfreport.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PdfReport pdfReport(QDir::homePath()+"/report.pdf");

    /*
   * QVariantList[
   * {
   *    "observation": QString observations,
   *    "results": QVariantMap statistics{
   *                  {"min", DBL_MAX},
   *                  {"max", DBL_MIN},
   *                  {"media", 0.0},
   *                  {"minTolerance", 0.0},
   *                  {"maxTolerance", 0.0},
   *                  {"status", false},
   *               };
   *
   *    "parameters": QVariantMap ventilatorParameters{
   *                   "key": "value",
   *                   "key1": "value1",
   *                   "key2": "value2",
   *                   "key3": "value3",
   *                  }
   *    "realResults": {
   *       16: [{"result": 10, "time": (10:45:51)}, {"result": 55.3, "time": (10:45:52)}, {"result": 8, "time": (10:45:53)}, {"result": 13.7, "time": (10:45:54)}...],
   *       7: [{"result": 10, "time": (10:45:51)}, {"result": 55.3, "time": (10:45:52)}, {"result": 8, "time": (10:45:53)}, {"result": 13.7, "time": (10:45:54)}...]
   * }, 
   * ...
   *
    */

    pdfReport.exportPdf(
        QVariantMap{
            {"testerName", "aaaaaa"},
            {"testerDate", "aaaaaa"},
            {"equipmentName", "aaaaaa"},
            {"testerRegistration", "aaaaaa"},
            {"hysteresisLabel", "aaaaaa"},
            {"equipmentNumber", "aaaaaa"}
        },
        QVariantList{
            QVariantMap{
                {"observation", "AAAAAAAA"},
                {"results", QVariantMap{
                    {"min", 15},
                    {"max", 20},
                    {"media", 0.0},
                    {"minTolerance", 0.0},
                    {"maxTolerance", 0.0},
                    {"status", false}
                    }
                },
                {"paremers", QVariantMap{
                    {"a", "b"},
                    {"c", "D"}
                    }
                }
            }
        }
    );

    qDebug()<<"Printou";

    return a.exec();
}
