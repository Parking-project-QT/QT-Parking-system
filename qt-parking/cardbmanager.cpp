#include "cardbmanager.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

bool CarDBManager::connectDB()
{
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    // The database path will be replaced with the deployment path later.
    m_db.setDatabaseName(QStringLiteral("parking.db"));

    if (!m_db.open()) {
        qWarning() << "DB 연결 실패:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral(
            "CREATE TABLE IF NOT EXISTS parking ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "car_number TEXT UNIQUE, "
            "in_time TEXT, "
            "is_deleted INTEGER DEFAULT 0 CHECK(is_deleted IN (0,1))"
            ")"))) {
        qWarning() << "parking 테이블 생성 실패:" << query.lastError().text();
        return false;
    }

    qDebug() << "DB 연결 성공:" << m_db.databaseName();
    return true;
}

bool CarDBManager::is_car_parked(const QString &carNumber, bool *queryOk) const
{
    if (queryOk != nullptr) {
        *queryOk = false;
    }

    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT 1 FROM parking "
        "WHERE car_number = :car_number AND is_deleted = 0"));
    query.bindValue(QStringLiteral(":car_number"), carNumber);

    if (!query.exec()) {
        qWarning() << "차량 조회 실패:" << query.lastError().text();
        return false;
    }

    if (queryOk != nullptr) {
        *queryOk = true;
    }
    return query.next();
}

bool CarDBManager::car_in(const QString &carNumber)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO parking (car_number, in_time, is_deleted) "
        "VALUES (:car_number, datetime('now','localtime'), 0) "
        "ON CONFLICT(car_number) DO UPDATE SET "
        "in_time = excluded.in_time, is_deleted = 0"));
    query.bindValue(QStringLiteral(":car_number"), carNumber);

    if (!query.exec()) {
        qWarning() << "입차 등록 실패:" << query.lastError().text();
        return false;
    }

    qDebug() << "입차 완료:" << carNumber;
    return true;
}

bool CarDBManager::car_out(const QString &carNumber)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "UPDATE parking SET is_deleted = 1 "
        "WHERE car_number = :car_number AND is_deleted = 0"));
    query.bindValue(QStringLiteral(":car_number"), carNumber);

    if (!query.exec()) {
        qWarning() << "출차 처리 실패:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() != 1) {
        qWarning() << "출차할 차량을 찾지 못함:" << carNumber;
        return false;
    }

    qDebug() << "출차 완료:" << carNumber;
    return true;
}

QList<QPair<QString, QDateTime>> CarDBManager::parkedCars() const
{
    QList<QPair<QString, QDateTime>> cars;
    QSqlQuery query(m_db);

    if (!query.exec(QStringLiteral(
            "SELECT car_number, in_time FROM parking "
            "WHERE is_deleted = 0 ORDER BY in_time, id"))) {
        qWarning() << "주차 차량 목록 조회 실패:" << query.lastError().text();
        return cars;
    }

    while (query.next()) {
        const QDateTime entryTime = QDateTime::fromString(
            query.value(1).toString(), QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        cars.append(qMakePair(query.value(0).toString(), entryTime));
    }

    return cars;
}
