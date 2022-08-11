#ifndef PDFREPORT_H
#define PDFREPORT_H

#include <QLineF>
#include <QRectF>
#include <QMarginsF>
#include <QPainter>
#include <QDateTime>
#include <QObject>
#include <QList>
#include <QLabel>
#include <QDebug>
#include <QDir>
#include <QPdfWriter>
#include "imagecontent.h"
#include "textcontent.h"
#include "tablepainter.h"
#include "chartpainter.h"

class PdfReport : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMarginsF pageMargins READ pageMargins WRITE setPageMargins NOTIFY pageMarginsChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

public:
    explicit PdfReport(QObject *parent = nullptr);
    explicit PdfReport(QString path);

    const QMarginsF &pageMargins() const;
    void setPageMargins(const QMarginsF &newPageMargins);

    const QString &fileName() const;
    void setFileName(const QString &newFileName);

    void exportPdf(QVariantMap header, QVariantList allData);

private:
    QPdfWriter* _writer;
    QPainter _painter;
    qreal _spaceUsedPage = 0.0;
    qreal _spacing = 0;
    qreal _painterWidth = 0;
    qreal _painterHeight = 0;
    qreal _endHeight = 0;
    qreal _remainingSpace = 0;
    QRectF _rect;
    QMarginsF _pageMargins = QMarginsF(10, 10, 10, 10);
    QString _fileName;

    void _changeLineWidth(qreal newLineWidth);
    void _chageLineColor(QColor newColor);

    void _getHeader(QVariantMap header);
    void _getParameterTableHeader(QString indexTest);
    void _getParameterTableLine(QVariantMap map, bool withColon = true);
    void _getParametersTable(QString indexTest, QVariantMap ventilatorParameters) ;
    void _getResultsTableLine(QStringList resultsData, QFont font = QFont());
    void _getResultsTable(QVariantList testResults);
    void _getChart(QString title, QVariantList testData);
    void _getObservation(QString observations);
    TablePainter _getSignatureField();
    TablePainter _getFooter();

    
    void _addElement(const AbstractPainter &element, qreal height, qreal width);
    void _addEnd(bool withSignature = false);
    void _addStart();
    void _addSpacing(int spacing = -1, bool checkRemainingSpace = true);
    void _checkAddElement(const AbstractPainter &element, qreal height, qreal width = 0);

    QStringList _getResultsData(QVariantMap map);
    qreal getRemainingSpace();
signals:

    void pageMarginsChanged();
    void fileNameChanged();
};

#endif // PDFREPORT_H
