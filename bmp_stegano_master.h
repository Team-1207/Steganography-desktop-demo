#ifndef BMPSTEGANOMASTER_H
#define BMPSTEGANOMASTER_H

#include <QImage>

class BitArray {
public:
    BitArray();
    BitArray(size_t cap);
    BitArray(QString text);
    QString get_text();
    void add_bit(uint8_t bit);
    void set_read_ptr(size_t offset);
    int get_next_bit(uint8_t &out);
    QVector<uint8_t> get_byte_arrray();
    size_t get_cur_pos();
private:
    size_t m_read_bit_ptr;
    size_t m_bit_len;
    QVector<uint8_t> m_bytes;
};

class BmpSteganoMaster {
public:
    BmpSteganoMaster();
    int set_img(QString f_name);
    int write_hidden_text(QString text, bool enableAlphaChannel);
    QString read_hidden_text();
    int save_img(QString filename);
private:
    QImage m_img;
    uint8_t m_set_last_bit(uint8_t byte, uint8_t bit);
    uint8_t m_get_last_bit(uint8_t byte);
};

#endif // BMPSTEGANOMASTER_H
