#include "formula.h"

// #include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#ARITHM!";
}

std::ostream& operator<<(std::ostream& output, FormulaException fe) {
    return output << "#FORMAT!";
}


namespace {
class Formula : public FormulaInterface {
public:
// Реализуйте следующие методы:

    // Конструктор формулы
    // Может выкинуть исключение при вводе лексически некорректной формулы
    // Обрабатываем это исключение прямо в списке инициализации (try catch).
    explicit Formula(std::string expression) try
        : ast_(ParseFormulaAST(expression)) {}
    catch(...)
    {
        throw FormulaException("Can\'t construct formula"s);
    }

    Value Evaluate(const SheetInterface&) const override {
        double res;
        try {
            res = ast_.Execute();
            return res;
        } catch (...) {
            return FormulaError("Arithmetic mistake"s);
        }
        // return res;
    }


    std::string GetExpression() const override {
        std::string res_str;
        std::ostringstream out(res_str);
        ast_.PrintFormula(out);
        res_str = out.str();
        return res_str;
    }

    // Возвращает список ячеек, на которые ссылается данная формула
    std::vector<Position> GetReferencedCells();

private:
    FormulaAST ast_;

    std::vector<Position> cells_; // список ячеек, присутствующих как операнды в формуле

};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}