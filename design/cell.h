#pragma once

#include "common.h"
#include "formula.h"

#include <optional>
#include <unordered_set>

class Sheet; // возможно, заглушка. Но если добавлять #include "sheet.h",  то будут перекрестные ссылки - не скомпилируется
/* 
Можно было бы хранить в Cell ссылку на SheetInterface, но SheetInterface методом GetCell 
может вернуть только объект класса CellInterface, а по текущему видению алгоритмов поиска 
циклических зависимостей и инвалидации, нам потребуются специфичные методы для Cell, 
которые отсутствуют в CellInterface (например, GetCellsReferencingToThis) 
*/


class Cell : public CellInterface {
public:
    explicit Cell(SheetInterface& sheet);

    ~Cell();

    /*
    Когда пользователь задаёт текст в методе Cell::Set(), 
    внутри метода определяется тип ячейки в зависимости от заданного текста 
    и создаётся нужный объект-реализация: формульная, текстовая, пустая.
    */
    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

    bool IsFormulaInCell();

    std::unordered_set<Position> GetCellsContainedInThis();
    std::unordered_set<Position> GetCellsReferencingToThis();
    
    // Методы для записи зависимостей 
    template<typename Iterator>
    void SetCellsContainedInThis(Iterator start, Iterator stop);
    
    template<typename Iterator>
    void SetCellsReferencingToThis(Iter start, Iter stop);

    // Проверяет, есть ли среди всех зависимостей от данной ячейки, 
    // ячейка с координатами pos. Необходим для обнаружения циклических зависимостей.
    // Если в данную ячейку пытаемся записать ячейку с позицией Y, 
    // то надо вызвать CheckExistingDependenciesOnThisCell(Y) => true - есть зависимость => цикличность
    bool CheckExistingDependenciesOnThisCell(Position pos);


private:
//можете воспользоваться нашей подсказкой, но это необязательно.
    class Impl;
    class EmptyImpl;
    class TextImpl;
    class FormulaImpl;

    std::unique_ptr<Impl> impl_;

    std::unordered_set<Position> cells_contained_in_this_;  // ячейки, на которые ссылается данная ячейка
    std::unordered_set<Position> cells_referencing_to_this_;  // ячейки, которые ссылаются на данную ячейку

    mutable std::optional<Value> cash_;  // храним результат расчета, чтобы не считать лишний раз 

    Sheet& sheet_;   // методы Cell могут менять содержимое таблицы
};