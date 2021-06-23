#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <QComboBox>


class FunctionSelector : public QComboBox
{
    Q_OBJECT

public:
    using Y_t = std::function<double(double, double)>;

    FunctionSelector(QWidget* parent, const std::map<std::string, Y_t>& functions);

    Y_t getFunction() const;

signals:
    void functionSelected(Y_t func);

protected slots:
    void selectionChanged();

private:
    std::vector<Y_t> functions;
};
