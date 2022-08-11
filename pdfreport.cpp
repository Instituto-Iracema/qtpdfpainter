#include "pdfreport.h"

PdfReport::PdfReport(QObject *parent) : QObject{parent}
{
}

PdfReport::PdfReport(QString path)
{
    qDebug() << "path:" << path;
    // path.remove(0,8);

    // int foldersUntilRoot = QDir::currentPath().remove(0, QDir::homePath().length()).count("/");

    // // path after home folders
    // path.remove(0, QDir::homePath().length());

    // // cleaning points and spaces from path, adding .pdf at end to prevent wrong extensions
    // path = path.split(".")[0].remove(QChar(' ')).append(".pdf");

    // // adding the backfolders because linux home shortcuts didn't work in qdocument
    // for(int i = 0; i < foldersUntilRoot; i ++){
    //     path = "../" + path;
    // }

    // qDebug() << "final path:" << path;

    _writer = new QPdfWriter(path);
    _writer->setPageSize(QPageSize(QPageSize::A4));
    _writer->setPageMargins(_pageMargins, QPageLayout::Unit::Millimeter);

    _painter.begin(_writer);
    _painter.setPen(QPen(QBrush(Qt::black), 1, Qt::SolidLine, Qt::RoundCap));
    _painterWidth = _painter.viewport().width();
    _painterHeight = _painter.viewport().height();
    _spacing = _painterHeight * .011;
    _endHeight = (6 * _spacing) + ((0.3/29.7) * _painterHeight) + ((1.0/29.7) * _painterHeight); // only footer
}

const QMarginsF &PdfReport::pageMargins() const
{
    return _pageMargins;
}

void PdfReport::setPageMargins(const QMarginsF &newPageMargins)
{
    if (_pageMargins == newPageMargins)
        return;
    _pageMargins = newPageMargins;
    emit pageMarginsChanged();
}

const QString &PdfReport::fileName() const
{
    return _fileName;
}

void PdfReport::setFileName(const QString &newFileName)
{
    if (_fileName == newFileName)
        return;
    _fileName = newFileName;

    emit fileNameChanged();
}

void PdfReport::exportPdf(QVariantMap header, QVariantList allData)
{
    const int sizeTests = allData.length();

    _spaceUsedPage = 0;

//    QVariantList[
//    * {
//    *    "observation": QString observations,
//    *    "results": QVariantMap statistics{
//    *                  {"min", DBL_MAX},
//    *                  {"max", DBL_MIN},
//    *                  {"media", 0.0},
//    *                  {"minTolerance", 0.0},
//    *                  {"maxTolerance", 0.0},
//    *                  {"status", false},
//    *               };
//    *
//    *    "parameters": QVariantMap ventilatorParameters{
//    *                   "key": "value",
//    *                   "key1": "value1",
//    *                   "key2": "value2",
//    *                   "key3": "value3",
//    *                  }
//    *    "realResults": {
//    *       16: [{"result": 10, "time": (10:45:51)}, {"result": 55.3, "time": (10:45:52)}, {"result": 8, "time": (10:45:53)}, {"result": 13.7, "time": (10:45:54)}...],
//    *       7: [{"result": 10, "time": (10:45:51)}, {"result": 55.3, "time": (10:45:52)}, {"result": 8, "time": (10:45:53)}, {"result": 13.7, "time": (10:45:54)}...]
//    * }, 


    _getHeader(header);
    // qDebug()<<header;

    QVariantMap myTest;

    for(auto i = 0; i < sizeTests; i++){
        myTest = allData.at(i).toMap();

        _addSpacing();
        _getParametersTable(QString::number(i+1) + "/" +  QString::number(sizeTests), myTest.value("parameters").toMap());

        QVariantMap results = myTest.value("realResults").toMap();
        for(auto j = 0; j<results.size(); j++){
            _addSpacing(_painterHeight * .0168);
            _getChart(results.keys().at(j), results.values().at(j).toList());
        }

        _addSpacing();
        _getResultsTable(myTest.value("results").toList());
        _addSpacing();
        _getObservation(myTest.value("observations").toString());

        if(i == (sizeTests - 1))
            _addEnd(true);
    }

    _painter.end();
    qDebug()<<"pdf terminou";
}

void PdfReport::_changeLineWidth(qreal newLineWidth)
{
    QPen newPen(_painter.pen().brush(), newLineWidth, Qt::SolidLine, Qt::RoundCap);
    _painter.setPen(newPen);
}

void PdfReport::_chageLineColor(QColor newColor)
{
    QPen newPen(QBrush(newColor), _painter.pen().widthF(), Qt::SolidLine, Qt::RoundCap);
    _painter.setPen(newPen);
}

void PdfReport::_getHeader(QVariantMap header)
{
    qDebug()<< "header";
    const auto contourLineWidth = 2.0;
    const auto defaultMargin = 0.02;

    TablePainter headerTable = TablePainter(); //TODO, resolver os bugs a partir daqui
    TableCellContent *content;
    TableCell *cell;

    TableRow *row = new TableRow();
        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new ImageContent(QImage(":magnamed_black.svg"), QMarginsF(.20, .28, .20, .28));
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            auto font = QFont();
            font.setWeight(QFont::Bold);
            font.setPixelSize(8);
            content = new TextContent("Relatório Ventmeter", Qt::AlignCenter, QMarginsF(.20, .28, .20, .28), false, Qt::black, font);
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Data e Hora:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm:ss"), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .401, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);
    headerTable.addRow(row);

    row = new TableRow(.15);
        cell = new TableCell(2.0/3.0, QColor(Qt::lightGray), contourLineWidth);
            content = new TextContent("Dados do Testador", Qt::AlignCenter, QMarginsF(defaultMargin, 0.15, defaultMargin, 0.15));
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::lightGray), contourLineWidth);
            content = new TextContent("Dados do Equipamento", Qt::AlignCenter, QMarginsF(defaultMargin, 0.15, defaultMargin, 0.15));
            cell->addContent(content);
        row->addCell(cell);
    headerTable.addRow(row);

    row = new TableRow();
        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Nome:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(header.value("testerName").toString(), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .4 + defaultMargin, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Data de Calibração:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(header.value("testerDate").toString(), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .4 + defaultMargin, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Nome:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(header.value("equipmentName").toString(), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .4 + defaultMargin, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);
    headerTable.addRow(row);

    row = new TableRow();
        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Número de Série:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(header.value("testerRegistration").toString(), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .4 + defaultMargin, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Modo:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(header.value("hysteresisLabel").toString(), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .4 + defaultMargin, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);

        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
            content = new TextContent("Número de Série:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
            cell->addContent(content);
            content = new TextContent(header.value("equipmentNumber").toString(), Qt::AlignBottom | Qt::AlignRight, QMarginsF(defaultMargin, .4 + defaultMargin, defaultMargin, defaultMargin));
            cell->addContent(content);
        row->addCell(cell);
    headerTable.addRow(row);

    qreal proportion = 2.4/29.7;
    _checkAddElement(headerTable, _painterHeight * proportion);
}

void PdfReport::_getParameterTableHeader(QString indexTest)
{
    qDebug()<< "param table header";
    const auto contourLineWidth = 2.0;
    const auto defaultMargin = 0.02;

    TablePainter tableHeader = TablePainter();
    TableCellContent *content;
    TableCell *cell;

    TableRow *row = new TableRow();
        cell = new TableCell(QColor(Qt::lightGray), contourLineWidth);
            content = new TextContent("Dados do Teste " + indexTest , Qt::AlignCenter, QMarginsF(defaultMargin, 0.15, defaultMargin, 0.15));
            cell->addContent(content);
        row->addCell(cell);
    tableHeader.addRow(row);
    

    qreal proportion = 0.4/29.7;
    _addSpacing();
    _checkAddElement(tableHeader, _painterHeight * proportion);

}

void PdfReport::_getParameterTableLine(QVariantMap map, bool withColon)
{
    qDebug()<< "param table line";
    const auto contourLineWidth = 2.0;
    const auto defaultMargin = 0.02;

    TablePainter tableLine = TablePainter();
    TableCellContent *content;
    TableCell *cell;

    TableRow *row = new TableRow();
        for(int i = 0; i < map.size(); i++){
            cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
                content = new TextContent(map.keys()[i] + ( withColon ? " : " : ""), Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, .6));
                cell->addContent(content);
                
                content = new TextContent(map.values()[i].toString(), Qt::AlignCenter, QMarginsF(defaultMargin, 0.25, defaultMargin, 0.25));
                cell->addContent(content);
                
            row->addCell(cell);
        }

    tableLine.addRow(row);

    qreal proportion = 0.7/29.7;
    _checkAddElement(tableLine, _painterHeight * proportion);
}

void PdfReport::_getParametersTable(QString indexTest, QVariantMap ventilatorParameters)
{
    qDebug() << "params table";
    _getParameterTableHeader(indexTest);

    if(!ventilatorParameters.size()){
        // empty line
        _getParameterTableLine(QVariantMap{{"", ""}}, false);
    } else {
        for(int i = 0; i < ventilatorParameters.size();) {
            QVariantMap miniMap;
            int countSplice = 0;

            while(countSplice < 3 && i <= (ventilatorParameters.size() - 1)){
                miniMap.insert(ventilatorParameters.keys()[i], ventilatorParameters.values()[i]);
                i++;
                countSplice++;
            }

            _getParameterTableLine(miniMap);
        }
    }

}

void PdfReport::_getResultsTableLine(QStringList resultsData, QFont font)
{
    qDebug()<< "results table line";
    const auto contourLineWidth = 2.0;
    const auto verticalMargins = 0.275;
    const auto horizontalMargins = 0.1;

    TablePainter resultsHeaderTable = TablePainter();

    TableCellContent *content;
    TableCell *cell;
    TableRow *row = new TableRow();

    for (int i = 0; i < resultsData.length(); i++) {
        cell = new TableCell(QColor(Qt::transparent), contourLineWidth);
        content = new TextContent(resultsData[i], Qt::AlignCenter, QMarginsF(horizontalMargins, verticalMargins, horizontalMargins, verticalMargins), false, Qt::black, font);
        cell->addContent(content);
        row->addCell(cell);
    }

    resultsHeaderTable.addRow(row);

    qreal proportion = 0.7/29.7;
    _checkAddElement(resultsHeaderTable, _painterHeight * proportion);
}

void PdfReport::_getResultsTable(QVariantList testResults)
{
    qDebug()<< "results";

    auto font = QFont();
    font.setWeight(QFont::Bold);
    QStringList resultsHeader{"Parâmetro", "Min", "Máx", "Média", "Limite Min", "Limite Máx", "OK/NOK"};

    _getResultsTableLine(resultsHeader, font);

    if(!testResults.length()){
        // empty line
        _getResultsTableLine(QStringList{"", "", "", "", "", "", ""});
    } else {
        for (int i = 0; i < testResults.length(); i++) {
            QStringList resultsData = _getResultsData(testResults.at(i).toMap());

            _getResultsTableLine(resultsData);
        }
    }
}

void PdfReport::_getChart(QString title, QVariantList testData)
{
    qDebug()<< "chart";
    ChartPainter chartpainter = ChartPainter();
    ChartPainterLineSeries* lineSeries = new ChartPainterLineSeries();

    const int gridHorizontalCount = 6;
    const int gridVerticalCount = 9;
    double yMin = testData.first().toMap().value("result").toDouble();
    double yMax = testData.first().toMap().value("result").toDouble();
    double xMax = 0;
    qreal proportion = 5/29.7;

    QDateTime initialDateTime = testData.at(0).toMap().value("time").toDateTime();
    for (int i = 0; i < testData.length(); i++) {
        QDateTime actualDateTime = testData.at(i).toMap().value("time").toDateTime();


        double yValue = testData.at(i).toMap().value("result").toDouble();
        double xValue = double(initialDateTime.msecsTo(actualDateTime)) / 1000;
        double xTruncValue = int(xValue * 100);

        xMax = xTruncValue / 100;
        if(yValue < yMin)
            yMin = yValue;
        if(yValue > yMax)
            yMax = yValue;

        lineSeries->addPoint(QPointF(xMax, yValue));
    }

    lineSeries->yMin = yMin;
    lineSeries->yMax = yMax;
    chartpainter.addLineSeries(*lineSeries);

    chartpainter.xMin = 0;
    chartpainter.xMax = xMax;
    chartpainter.gridHorizontalCount = gridHorizontalCount;
    chartpainter.gridVerticalCount = gridVerticalCount;
    chartpainter.title = title;

    _checkAddElement(chartpainter, _painterHeight * proportion);
}

void PdfReport::_getObservation(QString observations)
{
    qDebug()<< "observation";
    const auto contourLineWidth = 2.0;
    const auto defaultMargin = 0.02;
    qreal proportion = 4.6/29.7;

    TablePainter observationsTable = TablePainter();
    TableCellContent *content;
    TableCell *cell;

    TableRow *row = new TableRow();
    cell = new TableCell(QColor(Qt::transparent), contourLineWidth, QColor(Qt::blue), TableCellWalls::All);

        auto font = QFont();
        font.setPixelSize(125);
        content = new TextContent("Observações:", Qt::AlignTop | Qt::AlignLeft, QMarginsF(defaultMargin, defaultMargin, defaultMargin, defaultMargin), true , QColor(Qt::black), font);
        cell->addContent(content);

        QTextOption option;
        option.setWrapMode(QTextOption::WrapAnywhere);
        font.setPixelSize(150);
        content = new TextContent(observations, option , QMarginsF(defaultMargin, 0.1, defaultMargin, defaultMargin), true, QColor(Qt::black), font);
        cell->addContent(content);

    row->addCell(cell);
    observationsTable.addRow(row);

    _checkAddElement(observationsTable, _painterHeight * proportion);
}

TablePainter PdfReport::_getSignatureField()
{
    qDebug() << "signature";
    const auto contourLineWidth = 2.0;
    const auto defaultMargin = 0.02;

    TablePainter signature = TablePainter();
    TableCellContent *content;
    TableCell *cell;
    TableRow *row = new TableRow();
    cell = new TableCell(QColor(Qt::transparent), contourLineWidth, QColor(Qt::blue), TableCellWalls::Top);
        content = new TextContent("Assinatura", Qt::AlignCenter, QMarginsF(defaultMargin, defaultMargin, defaultMargin, defaultMargin));
        cell->addContent(content);
    row->addCell(cell);
    signature.addRow(row);

    return signature;
}

TablePainter PdfReport::_getFooter()
{
    qDebug() << "footer";
    const auto contourLineWidth = 2.0;
    const auto defaultMargin = 0.02;

    TablePainter footer = TablePainter();
    TableCellContent *content;
    TableCell *cell;
    TableRow *row = new TableRow();
    cell = new TableCell(QColor(Qt::transparent), contourLineWidth, QColor(Qt::blue), TableCellWalls::None);
        auto font = QFont();
        font.setWeight(QFont::Bold);
        font.setPixelSize(125);
        content = new TextContent("Ⓒ MAGNAMED® Tecnologia Médica S/A \n \"Faça Simples! Faça Melhor!\"", Qt::AlignCenter, QMarginsF(defaultMargin/1.8, defaultMargin, defaultMargin/1.8, defaultMargin), true, QColor(Qt::black), font);
        cell->addContent(content);
    row->addCell(cell);
    footer.addRow(row);

    return footer;

}

void PdfReport::_addElement(const AbstractPainter &element, qreal height, qreal width)
{
    if(width == 0){
        width = _painterWidth;
    }

    QRectF newRect((_painterWidth - width )/ 2, _spaceUsedPage, width, height);
    element.setRect(newRect);
    element.draw(&_painter);
    _spaceUsedPage += height;
}

void PdfReport::_addEnd(bool withSignature)
{
    qreal proportionH, proportionW;

    // qDebug()<< "remainingspace no end" << getRemainingSpace();
    _spaceUsedPage += getRemainingSpace() + 4 * _spacing;

    if(withSignature){
        proportionH = 0.3/29.7;
        proportionW = 11.0/21.0;
        _addElement(_getSignatureField(), proportionH * _painterHeight, proportionW * _painterWidth);
    }

    _spaceUsedPage += 2 * _spacing;
    proportionH = 1.0/29.7;
    proportionW = 6.0/21.0;
    _addElement(_getFooter(), proportionH * _painterHeight, proportionW * _painterWidth);
}

void PdfReport::_addStart()
{
    _writer->newPage();
    _spaceUsedPage = 0;
    qDebug() << "\n\nCRIANDO NOVA PAGINA\n\n";
}

void PdfReport::_addSpacing(int spacing, bool checkRemainingSpace)
{

    if(spacing < 0){
        spacing = getRemainingSpace() >= _spacing ? _spacing : 0;
    }

    if(checkRemainingSpace && (spacing > getRemainingSpace())){
        _addEnd();
        _addStart();
    } else {
        _spaceUsedPage += spacing;
    }
}

void PdfReport::_checkAddElement(const AbstractPainter &element, qreal height, qreal width)
{

    if(height <= (_painterHeight - _endHeight)){ 
        if(height > getRemainingSpace()){
            _addEnd();
            _addStart();
        }
        _addElement(element, height, width);
    }
}

QStringList PdfReport::_getResultsData(QVariantMap map)
{
    QStringList resultsData;

    resultsData.append(map.value("label").toString());
    resultsData.append(map.value("min").toString());
    resultsData.append(map.value("max").toString());
    resultsData.append(map.value("average").toString());
    resultsData.append(map.value("minTolerance").toString());
    resultsData.append(map.value("maxTolerance").toString());
    resultsData.append(map.value("status").toBool() ? "OK" : "NOK");

    return resultsData;
}

qreal PdfReport::getRemainingSpace()
{
    return _painterHeight - _spaceUsedPage - _endHeight;
}
