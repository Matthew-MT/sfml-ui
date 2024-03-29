#ifndef UI_TABLE_HPP
#define UI_TABLE_HPP

#include <SFML/Graphics.hpp>
#include "elements.hpp"
#include "table_h.hpp"

namespace UI {
    template<class ElementType, class BackgroundType> Table<ElementType, BackgroundType>::Table(
        const ArrayType& initElems, const sf::Vector2f& size, const sf::Vector2f& innerPad, const sf::Vector2f& outerPad
    ) : elements{initElems}, innerPadding{innerPad}, outerPadding{outerPad} {
        if (!std::is_base_of<sf::RectangleShape, BackgroundType>::value) throw "Invalid type specifier. Type must inherit from class sf::RectangleShape!";
        this->recomputeSizes();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::recomputePos() {
        sf::Vector2f pos = background.getPosition() + outerPadding;
        for (std::size_t i = 0u; i < elements.size(); i++)
            for (std::size_t j = 0u; j < elements[i].size(); j++)
                elements[i][j].setPosition(pos + sf::Vector2f{(calcedElementSize.x * (float)i) + (innerPadding.x * (float)i), (calcedElementSize.y * (float)j) + (innerPadding.y * (float)j)});
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::recomputeSizes() {
        calcedGridSize = sf::Vector2u{elements.size(), 0u};
        for (auto& column : elements) if (column.size() > calcedGridSize.y) calcedGridSize.y = column.size();

        calcedElementSize = background.getSize() - sf::Vector2f{(innerPadding.x * (float)(calcedGridSize.x - 1u)) + (outerPadding.x * 2.f), (innerPadding.y * (float)(calcedGridSize.y - 1u)) + (outerPadding.y * 2.f)};
        calcedElementSize.x = calcedElementSize.x / calcedGridSize.x;
        calcedElementSize.y = calcedElementSize.y / calcedGridSize.y;
        for (std::vector<ElementType>& column : elements) for (ElementType& element : column) {
            element.setSize(calcedElementSize);
        }
        this->recomputePos();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::setPosition(const sf::Vector2f& pos) {
        background.setPosition(pos);
        this->recomputePos();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::setSize(const sf::Vector2f& size) {
        background.setSize(size);
        this->recomputeSizes();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::setAllElements(const ArrayType& elems) {
        elements = elems;
        this->recomputeSizes();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::insert(const ElementType& inserted, const sf::Vector2i& place, bool lockY) {
        if (place.x > elements.size()) elements.push_back({inserted});
        else if (lockY && !place.y) elements.insert(place.x, 1, {inserted});
        else elements[place.x].insert(place.y, 1, inserted);
        this->recomputeSizes();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::erase(const sf::Vector2u& place) {
        if (!place.y && elements.at(place.x).size() == 1) elements.erase(place.x);
        else elements[place.x].erase(place.y);
        this->recomputeSizes();
    }

    template<class ElementType, class BackgroundType> void Table<ElementType, BackgroundType>::replaceElement(const ElementType& replaced, sf::Vector2i place) {
        if (place.x == -1) place.x = elements.size() - 1;
        if (place.y == -1) place.y = elements.at(place.x).size() - 1;
        elements[place.x][place.y] = replaced;
    }

    template<class ElementType, class BackgroundType> std::vector<std::vector<ElementType>>* Table<ElementType, BackgroundType>::getElements() {return &elements;}
    template<class ElementType, class BackgroundType> sf::Vector2f Table<ElementType, BackgroundType>::getPosition() const {return background.getPosition();}
    template<class ElementType, class BackgroundType> sf::Vector2f Table<ElementType, BackgroundType>::getSize() const {return background.getSize();}
    
    template<class ElementType, class BackgroundType> sf::Vector2u Table<ElementType, BackgroundType>::getTableSize() const {return calcedGridSize;}
};

#endif