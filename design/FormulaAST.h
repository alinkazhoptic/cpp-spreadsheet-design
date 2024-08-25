#pragma once

#include "FormulaLexer.h"
#include "common.h"

#include <forward_list>
#include <functional>
#include <stdexcept>

namespace ASTImpl {
// Узел дерева
class Expr;
}

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class FormulaAST {
public:
    explicit FormulaAST(std::unique_ptr<ASTImpl::Expr> root_expr);
    FormulaAST(FormulaAST&&) = default;
    FormulaAST& operator=(FormulaAST&&) = default;
    ~FormulaAST();

    double Execute(const SheetInterface&) const;
    void Print(std::ostream& out) const;
    void PrintFormula(std::ostream& out) const;

private:
    std::unique_ptr<ASTImpl::Expr> root_expr_;
};


/*
Cоздаёт дерево из текста формулы. 
Поочерёдно вызывает лексический и синтаксический анализаторы, 
а затем рекурсивно обходит дерево разбора и строит заготовку дерева для вычислений.
*/
// Этот метод поменяется, будет возвращать ещё список других ячеек, содержащихся в формуле 
FormulaAST ParseFormulaAST(std::istream& in);
FormulaAST ParseFormulaAST(const std::string& in_str);
