#include <functional>
#include "bmp_stegano_master.h"
#include <QDebug>

// ============================================================================
//                                    BmpSteganoMaster
// =============================================================================

BmpSteganoMaster::BmpSteganoMaster() {

}

/// \brief Установить картинку для дальней работы.
///
/// \param[in] Путь к изображению.
///
/// \return 0 - rgb, 1 - grba, -1 - ошибка.
int BmpSteganoMaster::set_img(QString f_name) {
    m_img = QImage(f_name);

    return 0;
}

/// \brief Выполнить действие для каждого пикселя картинки.
///
/// \param[in] img - Кртинка, которую требуется обойти.
/// \param[in] act - Действие, которое выполнится над каждым пикселем.
///
/// \return Код ошибки или 0 в случае успеха.
int for_each_pix(QImage& img, std::function<int(QRgb&)> act) {
    for (int y = 0; y < img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            QRgb &rgb = line[x];
            int ok = act(rgb);
            if (ok == -1) {
                return 0;
            }
        }
    }
    return -1;
}

/// \brief Застеганографировать текст в картинку (картинка устанавливается через set_img).
///
/// \param[in] text - Текст, подлежащий стеганографированию.
/// \param[in] enableAlphaChannel - Использовать ли Alpha-канал.
///
/// \return Код ошибкии или 0 в случае успеха.
int BmpSteganoMaster::write_hidden_text(QString text, bool enableAlphaChannel) {
    auto ba = BitArray(text);
    if (enableAlphaChannel) {
        m_img = m_img.convertToFormat(QImage::Format_ARGB32);
    } else {
        m_img = m_img.convertToFormat(QImage::Format_RGB32);
    }
    qDebug() << "[write_hidden_text] >> " << (m_img.hasAlphaChannel( ) ? "RGBA" : "RGB");
    for_each_pix(m_img, [this, &ba](QRgb& rgb) -> int {
        int ret = 0;
        uint8_t bits[4] {0, 0, 0, 0};
        int slots_n = 3 + (m_img.hasAlphaChannel() ? 1 : 0);
        for (int i = 0; i < slots_n; i++) {
            int ok = ba.get_next_bit(bits[i]);
            if (ok == -1) {
                ret = -1;
                bits[i] = 0;
            }
        }
        if (m_img.hasAlphaChannel()) {
            rgb = qRgba(m_set_last_bit(qRed(rgb), bits[0]),
                        m_set_last_bit(qGreen(rgb), bits[1]),
                        m_set_last_bit(qBlue(rgb), bits[2]),
                        m_set_last_bit(qAlpha(rgb), bits[3]));
        } else {
            rgb = qRgb(m_set_last_bit(qRed(rgb), bits[0]),
                        m_set_last_bit(qGreen(rgb), bits[1]),
                        m_set_last_bit(qBlue(rgb), bits[2]));
        }

        if (m_img.hasAlphaChannel( )) {
            qDebug() << QString( ).asprintf("[write_hidden_text] rgba <= 0x%.2X 0x%.2X 0x%.2X 0x%.2X",
                                           qRed(rgb), qGreen(rgb), qBlue(rgb), qAlpha(rgb)).toStdString( ).c_str( );
            qDebug() << "[write_hidden_text] bits: " << bits[0] << bits[1] << bits[2] << bits[3];
        } else {
            qDebug() << QString( ).asprintf("[write_hidden_text] rgb <= 0x%.2X 0x%.2X 0x%.2X",
                                           qRed(rgb), qGreen(rgb), qBlue(rgb)).toStdString( ).c_str( );
            qDebug() << "[write_hidden_text] bits: " << bits[0] << bits[1] << bits[2];
        }
        return ret;
    });
    return 0;
}

/// \brief Прочитать застеганографированный текст (картинка уставливается методом set_img)
///
/// \return Застеганогрвафированный текст
QString BmpSteganoMaster::read_hidden_text() {
    auto ba = BitArray(size_t((m_img.height() * m_img.width()) / 8) + 1);
    qDebug() << "[read_hidden_text] << " << (m_img.hasAlphaChannel( ) ? "RGBA" : "RGB");
    for_each_pix(m_img, [this, &ba](QRgb& rgb) -> int {
        uint8_t bits[4] = {0, 0, 0, 0};

        bits[0] = m_get_last_bit(qRed(rgb));
        ba.add_bit(bits[0]);

        bits[1] = m_get_last_bit(qGreen(rgb));
        ba.add_bit(bits[1]);

        bits[2] = m_get_last_bit(qBlue(rgb));
        ba.add_bit(bits[2]);

        if (m_img.hasAlphaChannel( )) {
            bits[3] = m_get_last_bit(qAlpha(rgb));
            ba.add_bit(bits[3]);
        }

        if (ba.get_cur_pos() < 20) {
            if (m_img.hasAlphaChannel( )) {
                qDebug( ) << QString( ).asprintf("[read_hidden_text] rgba => 0x%.2X 0x%.2X 0x%.2X 0x%.2X",
                                                 qRed(rgb), qGreen(rgb), qBlue(rgb), qAlpha(rgb)).toStdString().c_str();
                qDebug( ) << "[read_hidden_text] bits: " << bits[0] << bits[1] << bits[2] << bits[3];
            } else {
                qDebug( ) << QString( ).asprintf("[read_hidden_text] rgb => 0x%.2X 0x%.2X 0x%.2X",
                                                 qRed(rgb), qGreen(rgb), qBlue(rgb)).toStdString().c_str();
                qDebug( ) << "[read_hidden_text] bits: " << bits[0] << bits[1] << bits[2];
            }
        }
        return 0;
    });
    return ba.get_text();
}

/// \brief Сохранить картинку
///
/// \param[in] Путь сохранения
///
/// \return Код ошибки или 0 в случае ошибки
int BmpSteganoMaster::save_img(QString filename) {
    return m_img.save(filename, "PNG");
}


/// \brief Получить младший бит байта
///
/// \param[in] -
///
/// \return Последний бит байта (0 или 1)
uint8_t BmpSteganoMaster::m_get_last_bit(uint8_t byte) {
    return byte & ((uint8_t)1);
}


/// \brief
///
/// \param[in]
uint8_t BmpSteganoMaster::m_set_last_bit(uint8_t byte, uint8_t bit) {
    if (bit == 0) {
        return byte &= ~((uint8_t)1);
    } else {
        return byte |= ((uint8_t)1);
    }
}

// ===============================================================================
//                                       BitArray
// ===============================================================================

BitArray::BitArray() {
    m_bytes.reserve(1024);
    m_bit_len = 0;
    m_read_bit_ptr = 0;
}

BitArray::BitArray(size_t cap) {
    m_bytes.reserve(cap);
    m_bit_len = 0;
    m_read_bit_ptr = 0;
}

BitArray::BitArray(QString text) {
    std::string str = text.toStdString();
    m_bytes.reserve(str.size());
    std::copy(str.begin(), str.end(), std::back_inserter(m_bytes));
    m_bit_len = m_bytes.size() * 8;
    m_read_bit_ptr = 0;
}

QString BitArray::get_text() {
    QByteArray data;
    std::copy(m_bytes.begin() , m_bytes.end() , std::back_inserter(data));
    return data;
}

void BitArray::add_bit(uint8_t bit) {
    uint8_t added_bit = bit;
    if (bit != 0) {
        added_bit = 1;
    }

    if (m_bit_len % 8 == 0) {
        if (m_bytes.size() < 20) {
            qDebug( ) << "[BitArray::add_bit] size: " << m_bytes.size() << "; got ord: " << int(m_bytes.back());
        }
        m_bytes.append(0);
    }
    m_bytes.back() |= added_bit << (7 - (m_bit_len % 8));


    m_bit_len++;
    return;
}

void BitArray::set_read_ptr(size_t offset){
    m_read_bit_ptr = offset;
}

int BitArray::get_next_bit(uint8_t& out) {
    if (m_read_bit_ptr >= m_bit_len) {
        return -1;
    }
    uint8_t current_byte = m_bytes.at(m_read_bit_ptr / 8);
    uint8_t mask = 1 << (7 - (m_read_bit_ptr % 8));
    out = (current_byte & mask) == 0 ? 0 : 1;
    m_read_bit_ptr++;
    return 0;
}

size_t BitArray::get_cur_pos() {
    return m_bytes.size();
}

QVector<uint8_t> BitArray::get_byte_arrray() {
    return m_bytes;
}
