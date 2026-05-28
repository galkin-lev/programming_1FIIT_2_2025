//
// Created by lv on 5/28/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_JSONPROCESSORFACADE_H
#define LAB6_LOGIN_REGISTRATION_JSONPROCESSORFACADE_H


#include <memory>
#include <vector>
#include "features/profile/lab6_json_raii_smartpointer/ProductInterface.h"

struct ProcessingResult {
    bool success = false;
    QString errorMessage;
    int processedCount = 0;
    std::vector<std::unique_ptr<ProductInterface>> products;  // products via interface
    QString outputFile;
};

class JsonProcessorFacade {
public:
    static ProcessingResult process(const QString& inputPath, const QString& outputPath);
};


#endif //LAB6_LOGIN_REGISTRATION_JSONPROCESSORFACADE_H