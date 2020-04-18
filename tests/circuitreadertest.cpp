/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include <QtTest>

#include <QBuffer>
#include <QTextStream>

#include <memory>

#include "circuitreader.h"

static_assert(!std::is_copy_assignable<CircuitReader>::value, "Should not be copy assignable");
static_assert(!std::is_copy_constructible<CircuitReader>::value, "Should not be copy constructible");
static_assert(std::is_move_assignable<CircuitReader>::value, "Should be move assignable");
static_assert(std::is_move_constructible<CircuitReader>::value, "Should be move constructible");

class CircuitReaderTest : public QObject
{
    Q_OBJECT
private:
    std::unique_ptr<QIODevice> convertToDevice(const QString &fileContent)
    {
        auto buffer = new QBuffer;
        Q_ASSERT(buffer->open(QIODevice::WriteOnly));
        auto stream = std::make_unique<QTextStream>(buffer);
        *stream << fileContent;
        buffer->close();
        return std::unique_ptr<QIODevice>{buffer};
    }

private slots:
    void shouldHaveDefaultState()
    {
        // GIVEN
        auto reader = CircuitReader{};

        // THEN
        QCOMPARE(reader.device(), static_cast<QIODevice*>(nullptr));
        QVERIFY(reader.readMetaData().isEmpty());
        QVERIFY(reader.readCircuit().isEmpty());

        // WHEN
        auto buffer = std::make_unique<QBuffer>();
        reader = CircuitReader{buffer.get()};

        // THEN
        QCOMPARE(reader.device(), buffer.get());
        QVERIFY(reader.readMetaData().isEmpty());
        QVERIFY(reader.readCircuit().isEmpty());
    }

    void shouldDecodeDeviceContent_data()
    {
        QTest::addColumn<QString>("content");
        QTest::addColumn<CircuitReader::MetaData>("expectedMetaData");
        QTest::addColumn<CircuitModel>("expectedCircuit");


        QStringList content;
        QTest::newRow("empty") << content.join('\n')
                               << CircuitReader::MetaData{}
                               << CircuitModel{};



        content.clear();
        content << "---"
                << "author: John Doe"
                << "revision: 42"
                << "---";
        QTest::newRow("header only") << content.join('\n')
                                     << CircuitReader::MetaData{
                                            {"author", "John Doe"},
                                            {"revision", 42}
                                        }
                                     << CircuitModel{};

        content.clear();
        content << "---"
                << "author: John Doe"
                << ""
                << "revision: 42";
        QTest::newRow("header only no end mark") << content.join('\n')
                                                 << CircuitReader::MetaData{}
                                                 << CircuitModel{};
        content.clear();
        content << "author: John Doe"
                << ""
                << "revision: 42"
                << "---";
        QTest::newRow("header only no begin mark") << content.join('\n')
                                                   << CircuitReader::MetaData{}
                                                   << CircuitModel{};

        content.clear();
        content << "---"
                << "author: John Doe"
                << "revision: 42"
                << "name: HIIT"
                << "---";
        QTest::newRow("header only with name") << content.join('\n')
                                               << CircuitReader::MetaData{
                                                      {"author", "John Doe"},
                                                      {"name", "HIIT"},
                                                      {"revision", 42}
                                                  }
                                               << CircuitModel{"HIIT", {}};

        content.clear();
        content << "["
                << "    {"
                << "        \"text\": \"foo\","
                << "        \"duration\": 4000"
                << "    },"
                << "    {"
                << "        \"text\": \"bar\""
                << "    }"
                << "]";
        QTest::newRow("json only") << content.join('\n')
                                   << CircuitReader::MetaData{}
                                   << CircuitModel{};
        content.clear();
        content << "---"
                << "---"
                << "["
                << "    {"
                << "        \"text\": \"bar\""
                << "    },"
                << "    {"
                << "        \"text\": \"foo\","
                << "        \"duration\": 4000"
                << "    }"
                << "]";
        QTest::newRow("empty header") << content.join('\n')
                                      << CircuitReader::MetaData{}
                                      << CircuitModel{
                                            {},
                                            {
                                                {"bar"},
                                                {"foo", 4000},
                                            }
                                         };

        content.clear();
        content << "---"
                << "author: John Doe"
                << "revision: 42"
                << "name: HIIT"
                << "---"
                << "["
                << "    {"
                << "        \"text\": \"foo\","
                << "        \"duration\": 4000"
                << "    },"
                << "    {"
                << "        \"text\": \"bar\""
                << "    }"
                << "]";
        QTest::newRow("complete") << content.join('\n')
                                  << CircuitReader::MetaData{
                                         {"author", "John Doe"},
                                         {"name", "HIIT"},
                                         {"revision", 42}
                                     }
                                  << CircuitModel{
                                         "HIIT",
                                         {
                                             {"foo", 4000},
                                             {"bar"}
                                         }
                                     };


        content.clear();
        content << "---"
                << "author: John Doe"
                << "revision: 42"
                << "name: HIIT"
                << "---"
                << "["
                << "    {"
                << "        \"text\": \"foo\","
                << "        \"duration\": 4000"
                << "    },"
                << "    {"
                << "        \"text\": \"One Loop\","
                << "        \"count\": 3,"
                << "        \"content\": ["
                << "            {"
                << "                \"text\": \"bar\""
                << "            },"
                << "            {"
                << "                \"text\": \"baz\","
                << "                \"duration\": 500"
                << "            }"
                << "        ]"
                << "    },"
                << "    {"
                << "        \"text\": \"bleh\""
                << "    },"
                << "    {"
                << "        \"count\": 4,"
                << "        \"removeLast\": true,"
                << "        \"content\": ["
                << "            {"
                << "                \"text\": \"baz\""
                << "            },"
                << "            {"
                << "                \"text\": \"bar\","
                << "                \"duration\": 400"
                << "            }"
                << "        ]"
                << "    }"
                << "]";
        QTest::newRow("complete with loops") << content.join('\n')
                                             << CircuitReader::MetaData{
                                                    {"author", "John Doe"},
                                                    {"name", "HIIT"},
                                                    {"revision", 42}
                                                }
                                             << CircuitModel{
                                                    "HIIT",
                                                    {
                                                        {"foo", 4000},
                                                        {"One Loop, bar (1/3)"},
                                                        {"One Loop, baz (1/3)", 500},
                                                        {"One Loop, bar (2/3)"},
                                                        {"One Loop, baz (2/3)", 500},
                                                        {"One Loop, bar (3/3)"},
                                                        {"One Loop, baz (3/3)", 500},
                                                        {"bleh"},
                                                        {"baz (1/4)"},
                                                        {"bar (1/4)", 400},
                                                        {"baz (2/4)"},
                                                        {"bar (2/4)", 400},
                                                        {"baz (3/4)"},
                                                        {"bar (3/4)", 400},
                                                        {"baz (4/4)"}
                                                    }
                                               };
    }

    void shouldDecodeDeviceContent()
    {
        // GIVEN
        QFETCH(QString, content);
        QFETCH(CircuitReader::MetaData, expectedMetaData);
        QFETCH(CircuitModel, expectedCircuit);

        auto device = convertToDevice(content);
        auto reader = CircuitReader{device.get()};

        // WHEN
        auto metaData = reader.readMetaData();

        // THEN
        QCOMPARE(reader.device(), device.get());
        QCOMPARE(metaData, expectedMetaData);
        if (!content.isEmpty()) {
            QVERIFY(device->pos() != 0);
        }

        // WHEN
        auto circuit = reader.readCircuit();

        // THEN
        QCOMPARE(circuit, expectedCircuit);
        QVERIFY(device->atEnd());

        // Now try circuit first, then metadata

        // GIVEN
        device = convertToDevice(content);
        reader = CircuitReader{device.get()};

        // WHEN
        circuit = reader.readCircuit();
        metaData = reader.readMetaData();

        // THEN
        QCOMPARE(reader.device(), device.get());
        QCOMPARE(metaData, expectedMetaData);
        QCOMPARE(circuit, expectedCircuit);
        QVERIFY(device->atEnd());
    }
};

QTEST_APPLESS_MAIN(CircuitReaderTest)

#include "circuitreadertest.moc"
