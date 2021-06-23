#include "functionselector.hpp"


FunctionSelector::FunctionSelector(QWidget* parent, const std::map<std::string, Y_t>& functions)
    : QComboBox(parent)
{
    for (const auto& [key, val] : functions)
    {
        this->functions.push_back(val);
        addItem(QString::fromStdString(key));
    }

    connect(this, &QComboBox::currentIndexChanged, this, &FunctionSelector::selectionChanged);
}

FunctionSelector::Y_t FunctionSelector::getFunction() const
{
    return functions[currentIndex()];
}

void FunctionSelector::selectionChanged()
{
    emit functionSelected(getFunction());
}

