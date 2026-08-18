#ifndef CARDBMANAGER_H
#define CARDBMANAGER_H

#include <QDateTime>
#include <QList>
#include <QPair>
#include <QSqlDatabase>
#include <QString>

class CarDBManager
{
public:
    bool connectDB();
    bool is_car_parked(const QString &carNumber, bool *queryOk = nullptr) const;
    bool car_in(const QString &carNumber);
    bool car_out(const QString &carNumber);

    QList<QPair<QString, QDateTime>> parkedCars() const;

private:
    QSqlDatabase m_db;
};

#endif
