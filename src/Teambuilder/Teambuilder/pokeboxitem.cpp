#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include "pokeboxitem.h"
#include "pokebox.h"
#include "../PokemonInfo/pokemonstructs.h"
#include "../PokemonInfo/pokemoninfo.h"

Q_DECLARE_METATYPE(PokeBox*)

PokeBoxItem::PokeBoxItem(PokeTeam *poke, PokeBox *box) : poke(NULL), m_Box(box)
{
    changePoke(poke);
    setFlags(QGraphicsItem::ItemIsMovable);
}

void PokeBoxItem::changePoke(PokeTeam *poke)
{
    delete this->poke;
    this->poke = poke;
    setPixmap(PokemonInfo::Icon(poke->num()));
}

void PokeBoxItem::setBox(PokeBox *newBox)
{
    m_Box = newBox;
}

PokeBoxItem::~PokeBoxItem()
{
    delete poke, poke = NULL;
}

void PokeBoxItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int distance = int((event->pos()-startPos).manhattanLength());
        if(distance >= QApplication::startDragDistance())
        {
            startDrag();
        }
    }
}

void PokeBoxItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isUnderMouse()) {
        event->ignore();
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        startPos = event->pos();
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void PokeBoxItem::startDrag()
{
    QMimeData * data = new QMimeData();
    QVariant v;
    v.setValue(m_Box);
    data->setProperty("Box", v);
    data->setProperty("Item", m_Box->getNumOf(this));
    data->setImageData(pixmap());
    QDrag * drag = new QDrag(m_Box->parentWidget());
    drag->setMimeData(data);
    drag->setPixmap(pixmap());
    drag->setHotSpot(QPoint(pixmap().width()/2,pixmap().height()/2));
    drag->exec(Qt::MoveAction);
}