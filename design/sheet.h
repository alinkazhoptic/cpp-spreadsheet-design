#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <map>

class Sheet : public SheetInterface {
public:
    Sheet()
    {   
        try{
            sheet_.reserve(Position::MAX_ROWS);

        } catch (const std::exception & err) {
            std::cout << "ERROR IN Sheet" << std::endl;
            std::cout << err.what() << std::endl;
        }
    }

    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    /*
    Ячейка может быть удалена или очищена. Если на неё ссылаются элементы - очищать (= EmptyCell)
    Если ссылок на нее нет, то удалить.
    */
    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

    // Эти методы нужны, чтобы иметь доступ к специфическим методам класса Cell,
    // которые не доступны через CellInterface
    const Cell* GetConcreteCell(Position pos) const;
    Cell* GetConcreteCell(Position pos);


private:

    Size printable_size_;
    std::map<int, int> rows_volume;  // кол-во ячеек в строке
    std::map<int, int> cols_volume;  // кол-во ячеек в столбце

    using Table = std::vector<std::vector<std::unique_ptr<CellInterface>>>;
    Table sheet_; 

    bool IsPositionInsidePrintableZone(Position pos) const;

    // Очистить кэш у ячеек, зависящих от ячейки на заданной позиции pos
    // Необходимо вызвать после валидного изменения ячейки pos 
    void ClearCashAtDepenpentCells(Position pos);

    // Обновляет граф при изменении заданной ячейки pos
    void AddConnections(Position pos); 

};