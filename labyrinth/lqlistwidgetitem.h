#ifndef LQLISTWIDGETITEM_H
#define LQLISTWIDGETITEM_H

#include <QListWidgetItem>
#include "normalid.h"


class LQListWidgetItem : public QListWidgetItem
{
private:
    NormalID normal;

public:
    LQListWidgetItem();
    LQListWidgetItem(NormalID normal){this->normal=normal;}
    NormalID *getNormal(){return &normal;}
};

#endif // LQLISTWIDGETITEM_H
