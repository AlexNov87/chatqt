#ifndef TOKENIZER_H
#define TOKENIZER_H
/**
 * @file tokenizer.h
 * @brief Заголовочный файл для класса TokenGen, предоставляющего функциональность для генерации шестнадцатеричных токенов.
 */
#include <random>
#include <sstream>
#include <iomanip>

namespace Service {

/**
 * @brief Класс, предоставляющий функциональность для генерации шестнадцатеричных токенов.
 */
class TokenGen
{
public:

    /**
     * @brief Генерирует шестнадцатеричный токен.
     *
     * Генерирует случайный шестнадцатеричный токен, состоящий из двух частей,
     * каждая из которых является шестнадцатеричным представлением 64-битного целого числа.
     *
     * @return Строка, представляющая сгенерированный шестнадцатеричный токен.
     */
    std::string GenerateHEXToken();
private:


    /**
     * @brief Преобразует 64-битное целое число в шестнадцатеричную строку.
     *
     * Преобразует заданное 64-битное целое число в его шестнадцатеричное представление.
     * Строка заполняется нулями слева до 16 символов.
     *
     * @param num 64-битное целое число для преобразования.
     * @return Строка, представляющая шестнадцатеричное представление числа.
     */
    std::string NumToHex(uint64_t num);

    /**
     * @brief Генератор случайных чисел.
     *
     * Используется для инициализации генераторов Mersenne Twister.
     */
    std::random_device random_device_;

    /**
     * @brief Первый генератор Mersenne Twister.
     *
     * Используется для генерации первой части токена.
     */
    std::mt19937_64 generator1_{[this]
                                {
                                    std::uniform_int_distribution<std::mt19937_64::result_type> dist;
                                    return dist(random_device_);
                                }()};
    /**
     * @brief Второй генератор Mersenne Twister.
     *
     * Используется для генерации второй части токена.
     */
    std::mt19937_64 generator2_{[this]
                                {
                                    std::uniform_int_distribution<std::mt19937_64::result_type> dist;
                                    return dist(random_device_);
                                }()};
};

}

#endif // TOKENIZER_H
