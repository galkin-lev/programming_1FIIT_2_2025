//
// Created by lv on 5/28/26.
//

#include "JsonProcessorFacade.h"

#include "JsonProcessorFacade.h"
#include "../JsonFileReader.h"
#include "../JsonFileWriter.h"
#include "../ProductFactory.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "../../../logger/logger.h"



///  in facade  - we read json (outputArray) -> edit it (call fabric) (edited outputArray) -> write it (edited outputArray) ///
///
ProcessingResult JsonProcessorFacade::process(const QString& inputPath, const QString& outputPath) {
    ProcessingResult result;

    LOG_INFO("=== JSON Warehouse Processing (Variant 2) ===");
    LOG_INFO("Input: " + inputPath.toStdString());

    // RAII 1
    JsonFileReader reader(inputPath);
    if (!reader.isOpen()) {
        result.errorMessage = "Cannot open input file: " + inputPath;
        LOG_ERROR(result.errorMessage.toStdString());
        return result;
    }

    QByteArray jsonData = reader.readAll();
    if (jsonData.isEmpty()) {
        result.errorMessage = "Input file is empty";
        LOG_ERROR(result.errorMessage.toStdString());
        return result;
    }

    // parse JSON
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        result.errorMessage = "JSON parse error: " + parseError.errorString();
        LOG_ERROR(result.errorMessage.toStdString());
        return result;
    }

    QJsonArray items = doc.array();
    QJsonArray outputArray;

    // once json read -> do smth with  fabric and send it
    for (const QJsonValue& value : items) {
        QJsonObject obj = value.toObject();
        auto product = ProductFactory::create(obj);
        if (product) {
            outputArray.append(product->toJson());
            result.products.push_back(std::move(product));
            result.processedCount++;
        }
    }

    // RAII 2 - write
    JsonFileWriter writer(outputPath);
    if (!writer.isOpen()) {
        result.errorMessage = "Cannot open output file: " + outputPath;
        LOG_ERROR(result.errorMessage.toStdString());
        return result;
    }

    QJsonDocument outputDoc(outputArray);
    if (!writer.write(outputDoc.toJson(QJsonDocument::Indented))) {
        result.errorMessage = "Failed to write output file";
        LOG_ERROR(result.errorMessage.toStdString());
        return result;
    }

    result.success = true;
    result.outputFile = outputPath;

    LOG_INFO("Processing completed. Fresh products: " + std::to_string(result.processedCount));
    return result;
}