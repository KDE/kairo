/* This file is part of Kairo Timer

   Copyright 2016 Kevin Ottens <ervin@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.
*/

#include <QtTest>

#include <memory>

#include "circuitlistmodel.h"

static_assert(std::is_base_of<QAbstractItemModel, CircuitListModel>::value, "Should be an item model");

class CircuitListModelTest : public QObject
{
    Q_OBJECT
private:
    QDir storageDir() const
    {
        auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        return {path};
    }

    void writeInFile(const QString &fileName, const QString &fileContent)
    {
        auto dir = storageDir();
        auto file = std::make_unique<QFile>(dir.path() + '/' + fileName);
        if (file->open(QFile::WriteOnly)) {
            auto stream = std::make_unique<QTextStream>(file.get());
            *stream << fileContent;
        }
        QTest::qWait(20);
    }

    void deleteFile(const QString &fileName)
    {
        QFile::remove(storageDir().path() + '/' + fileName);
        QTest::qWait(20);
    }

private slots:
    void initTestCase()
    {
        QCoreApplication::setApplicationName("circuitlistmodeltest");
        QStandardPaths::setTestModeEnabled(true);
        cleanupTestCase();
    }

    void cleanupTestCase()
    {
        auto dir = storageDir();
        if (dir.exists()) {
            dir.removeRecursively();
            dir.rmpath("..");
        }
    }

    void shouldHaveDefaultStateAndCreateStorageDir()
    {
        // GIVEN
        auto dir = storageDir();
        QVERIFY(!dir.exists());
        auto model = std::make_unique<CircuitListModel>();

        // THEN
        QVERIFY(dir.exists());
        QCOMPARE(model->path(), dir.path());
        QCOMPARE(model->rowCount(), 0);
        QCOMPARE(model->roleNames().value(Qt::DisplayRole), QByteArray("display"));
        QCOMPARE(model->roleNames().value(CircuitListModel::FilePathRole), QByteArray("filePath"));
    }

    void shouldListCircuitsInStorageDir()
    {
        // GIVEN
        auto model = std::make_unique<CircuitListModel>();
        auto spy = std::make_unique<QSignalSpy>(model.get(), &CircuitListModel::modelReset);

        // WHEN
        writeInFile("foo.kairo", "---\nname: Foo Circuit\n---\n[]\n");

        // THEN
        QCOMPARE(spy->size(), 1);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 1);
        QCOMPARE(model->rowCount(model->index(0, 0)), 0);
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("Foo Circuit"));
        QCOMPARE(model->data(model->index(0, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "foo.kairo");
        spy->clear();

        // WHEN
        writeInFile("ignored.json", "---\nname: Ignored file (not .kairo)\n---\n[]\n");

        // THEN
        QCOMPARE(spy->size(), 0);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 1);
        QCOMPARE(model->rowCount(model->index(0, 0)), 0);
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("Foo Circuit"));
        QCOMPARE(model->data(model->index(0, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "foo.kairo");
        spy->clear();

        // WHEN
        writeInFile("bar.kairo", "---\nname: ZBar\n---\n[]\n");

        // THEN
        QCOMPARE(spy->size(), 1);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 2);
        QCOMPARE(model->rowCount(model->index(0, 0)), 0);
        QCOMPARE(model->rowCount(model->index(1, 0)), 0);
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("Foo Circuit"));
        QCOMPARE(model->data(model->index(0, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "foo.kairo");
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("ZBar"));
        QCOMPARE(model->data(model->index(1, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "bar.kairo");
        spy->clear();

        // WHEN
        writeInFile("zab.kairo", "---\nname: Baz Circuit\n---\n[]\n");

        // THEN
        QCOMPARE(spy->size(), 1);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 3);
        QCOMPARE(model->rowCount(model->index(0, 0)), 0);
        QCOMPARE(model->rowCount(model->index(1, 0)), 0);
        QCOMPARE(model->rowCount(model->index(2, 0)), 0);
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("Baz Circuit"));
        QCOMPARE(model->data(model->index(0, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "zab.kairo");
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("Foo Circuit"));
        QCOMPARE(model->data(model->index(1, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "foo.kairo");
        QCOMPARE(model->data(model->index(2, 0)).toString(), QString("ZBar"));
        QCOMPARE(model->data(model->index(2, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "bar.kairo");
        spy->clear();

        // WHEN
        deleteFile("foo.kairo");

        // THEN
        QCOMPARE(spy->size(), 1);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 2);
        QCOMPARE(model->rowCount(model->index(0, 0)), 0);
        QCOMPARE(model->rowCount(model->index(1, 0)), 0);
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("Baz Circuit"));
        QCOMPARE(model->data(model->index(0, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "zab.kairo");
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("ZBar"));
        QCOMPARE(model->data(model->index(1, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "bar.kairo");
        spy->clear();

        // WHEN
        deleteFile("bar.kairo");

        // THEN
        QCOMPARE(spy->size(), 1);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 1);
        QCOMPARE(model->rowCount(model->index(0, 0)), 0);
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("Baz Circuit"));
        QCOMPARE(model->data(model->index(0, 0), CircuitListModel::FilePathRole).toString(),
                 storageDir().path() + '/' + "zab.kairo");
        spy->clear();

        // WHEN
        deleteFile("zab.kairo");

        // THEN
        QCOMPARE(spy->size(), 1);
        QCOMPARE(model->path(), storageDir().path());
        QCOMPARE(model->rowCount(), 0);
        spy->clear();
    }

    void shouldLoadCircuits()
    {
        // GIVEN
        auto model = std::make_unique<CircuitListModel>();
        writeInFile("foo.kairo", "---\nname: Foo Circuit\n---\n[{\"text\": \"foo\"},{\"text\": \"foo\", \"duration\": 800}]\n");
        writeInFile("bar.kairo", "---\nname: Bar Circuit\n---\n[{\"text\": \"bar\", \"duration\": 4000},{\"text\": \"bar\"}]\n");

        // WHEN
        auto circuit = model->loadCircuit(0);

        // THEN
        auto expected = CircuitModel{"Bar Circuit",{{"bar", 4000}, {"bar"}}};
        QCOMPARE(circuit, expected);

        // WHEN
        circuit = model->loadCircuit(1);

        // THEN
        expected = CircuitModel{"Foo Circuit",{{"foo"}, {"foo", 800}}};
        QCOMPARE(circuit, expected);
    }
};

QTEST_MAIN(CircuitListModelTest)

#include "circuitlistmodeltest.moc"
