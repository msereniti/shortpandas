
#include "../csv-reader/csv-reader.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>


struct Account // структура для хранения количетва строк на страну
{
    std::string country;
    int point_beg; //начало вхождения в страну
    int point_end; //последнее значение страны
};

/**
@brief Вычисление минимального, максимального значений, медианы и квартилей
@detailed Данная функция устанавливает максимальное и минимальное значения после
сортировки вектора и вычисляет медиану и квартили через нахождение номера нужной
ячейки вектора, а после записывает значения в параметры функции
@param Вектор с частью выборки, индекс первой ячейки стрыны, индекс последней ячейки
страны, вектор для записи минимальных значений, вектор для записи максимальных значений,
вектор для записи медиан, вектор для записи квартилей 25% и 75%
@return Не возвращает значений
*/

void Simple_calculation (std::vector<long double> array,
                         int beg,
                         int end,
                         long double &min,
                         long double &max,
                         long double &median,
                         long double &quartile_25,
                         long double &quartile_75)
{
    int quantity = end - beg + 1; // количество строк
    long double num_quart = 0; //коэффициент для подсчета квартелей
    min = array.at (0); // минимум
    max = array.at (quantity - 1); // максимум
    if (quantity % 2 != 0)
    {
        median = array.at ((quantity + 1) / 2 - 1); // медиана для нечетного количества значений в стране
    }
    else
        median = (array.at (quantity / 2 - 1) + array.at ((quantity / 2 + 1) / 2 - 1)); // для четного количества значений в стране
    num_quart = (quantity + 1) * (0.25);
    if (num_quart == round (num_quart))
    {
        quartile_25 = array.at (num_quart); //если коэф целый
    }
    else
    {
        quartile_25 = (array.at (floor (num_quart) - 1) + array.at (ceil (num_quart) - 1)) / 2; //если коэф дробный
    }

    num_quart = (quantity + 1) * (0.75);
    if (num_quart == round (num_quart))
    {
        quartile_75 = array.at (num_quart);
    }
    else
    {
        quartile_75 = (array.at (floor (num_quart) - 1) + array.at (ceil (num_quart) - 1)) / 2;
    }
}


/**
@brief Вычисление среднего значения и среднеквадратичного отклонения
@detailed Данная функция вычисляет среднеее значение по стране и
среднеквадратичное отклонение на основе полученного среднего значения,
а после записывает значения в параметры функции.
@param Вектор с частью выборки, индекс первой ячейки стрыны, индекс последней ячейки
страны, сумма всех ячеек столбца по стране, вектор для записи среднеквадратичного
отклонения и среднего значения
@return Не возвращает значений
*/

void Square_dev_calculation (std::vector<long double> array,
                             int beg,
                             int end,
                             long double sum,
                             long double &square_deviation,
                             long double &average)
{
    int quantity = end - beg + 1; //количество строк
    average = sum / quantity; // среднее значение по стране
    sum = 0;
    for (int i = 0; i < array.size (); i++)
    {
        sum += pow ((array.at (i) - average), 2); // сумма квадратов разностей
    }
    square_deviation = sqrt (sum / (quantity - 1)); //среднеквадратичное отклонение
}


/**
@brief Обработка выборки и вычисление статистических характеристик
@detailed Данная функция записывает в вектор часть выборки и отправляет
его в качестве аргумента в другие функциия для вычисление статистических
характеристик, а после записывает вектора со всеми полученными значениями
в параметры функции.
@param Выборка данных,вектора для записи: минимальных, максимальных,
средних значений, медианы, квартилей 25% и 75%, среднеквадратичного
отклонения
@return Не возвращает значений
*/


int statisticalComputations (std::vector<InputRow> data,
                             std::vector<InputRow> &min,
                             std::vector<InputRow> &max,
                             std::vector<InputRow> &average,
                             std::vector<InputRow> &median,
                             std::vector<InputRow> &quartile_25,
                             std::vector<InputRow> &quartile_75,
                             std::vector<InputRow> &square_deviation)
{


    std::vector<Account> counter;
    std::vector<long double> array;
    int num_countr = 0; //индекс текущей страны
    for (int i = 0; i < data.size (); i++)
    {
        array.clear (); //очистка вектора
        InputRow sum = { "unknown", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //модель для добавления в вектор
        sum.country = data.at (i).country; //страна, которая сейчас обрабатывается
        Account nulln = { "unknown", 0, 0 };
        //добавление значение в конец векторов
        counter.push_back (nulln);
        min.push_back (sum);
        max.push_back (sum);
        average.push_back (sum);
        median.push_back (sum);
        quartile_25.push_back (sum);
        quartile_75.push_back (sum);
        square_deviation.push_back (sum);
        counter.at (num_countr).point_beg = i; //первое вхождения в страну

        if (num_countr != 0 && data.at (i - 1).country != data.at (i - 2).country) //учитываем строку при переходе на другую страну
        {
            i--;
            counter.at (num_countr).point_beg = i;
        }
        do
        {
            array.push_back (data.at (i).crop_land); //добавлние значения в вектор
            sum.crop_land += data.at (i).crop_land; // вычисление суммы всех значений столбца
            i++;
        } while (data.at (i).country == data.at (i - 1).country &&
                 i != data.size () - 1); //цикл, чтобы не выйти за пределы страны
        counter.at (num_countr).point_end = i - 1;
        counter.at (num_countr).country = data.at (i - 1).country;
        if (i == data.size () - 1) //для последнего значения в главном векторе
        {
            array.push_back (data.at (i).crop_land);
            sum.crop_land += data.at (i).crop_land;
            counter.at (num_countr).point_end = i++;
        }

        sort (array.begin (), array.end ()); //сортировка по возрастанию
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).crop_land, max.at (num_countr).crop_land,
                            median.at (num_countr).crop_land, quartile_25.at (num_countr).crop_land,
                            quartile_75.at (num_countr).crop_land);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.crop_land, square_deviation.at (num_countr).crop_land,
                                average.at (num_countr).crop_land);

        array.clear (); //очистка вектора для нового использования
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++) // цикл для записи значений в вектор
        {
            array.push_back (data.at (j).grazing_land);
            sum.grazing_land += data.at (j).grazing_land;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).grazing_land, max.at (num_countr).grazing_land,
                            median.at (num_countr).grazing_land, quartile_25.at (num_countr).grazing_land,
                            quartile_75.at (num_countr).grazing_land);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.grazing_land, square_deviation.at (num_countr).grazing_land,
                                average.at (num_countr).grazing_land);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).forest_land);
            sum.forest_land += data.at (j).forest_land;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).forest_land, max.at (num_countr).forest_land,
                            median.at (num_countr).forest_land, quartile_25.at (num_countr).forest_land,
                            quartile_75.at (num_countr).forest_land);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.forest_land, square_deviation.at (num_countr).forest_land,
                                average.at (num_countr).forest_land);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).fishing_ground);
            sum.fishing_ground += data.at (j).fishing_ground;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).fishing_ground, max.at (num_countr).fishing_ground,
                            median.at (num_countr).fishing_ground, quartile_25.at (num_countr).fishing_ground,
                            quartile_75.at (num_countr).fishing_ground);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.fishing_ground, square_deviation.at (num_countr).fishing_ground,
                                average.at (num_countr).fishing_ground);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).built_up_land);
            sum.built_up_land += data.at (j).built_up_land;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).built_up_land, max.at (num_countr).built_up_land,
                            median.at (num_countr).built_up_land, quartile_25.at (num_countr).built_up_land,
                            quartile_75.at (num_countr).built_up_land);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.built_up_land, square_deviation.at (num_countr).built_up_land,
                                average.at (num_countr).built_up_land);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).carbon);
            sum.carbon += data.at (j).carbon;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg,
                            counter.at (num_countr).point_end, min.at (num_countr).carbon,
                            max.at (num_countr).carbon, median.at (num_countr).carbon,
                            quartile_25.at (num_countr).carbon, quartile_75.at (num_countr).carbon);
        Square_dev_calculation (array, counter.at (num_countr).point_beg,
                                counter.at (num_countr).point_end, sum.carbon,
                                square_deviation.at (num_countr).carbon, average.at (num_countr).carbon);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).total);
            sum.total += data.at (j).total;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg,
                            counter.at (num_countr).point_end, min.at (num_countr).total,
                            max.at (num_countr).total, median.at (num_countr).total,
                            quartile_25.at (num_countr).total, quartile_75.at (num_countr).total);
        Square_dev_calculation (array, counter.at (num_countr).point_beg,
                                counter.at (num_countr).point_end, sum.total,
                                square_deviation.at (num_countr).total, average.at (num_countr).total);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).percapita);
            sum.percapita += data.at (j).percapita;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).percapita, max.at (num_countr).percapita,
                            median.at (num_countr).percapita, quartile_25.at (num_countr).percapita,
                            quartile_75.at (num_countr).percapita);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.percapita, square_deviation.at (num_countr).percapita,
                                average.at (num_countr).percapita);

        array.clear ();
        for (int j = counter.at (num_countr).point_beg; j <= counter.at (num_countr).point_end; j++)
        {
            array.push_back (data.at (j).population);
            sum.population += data.at (j).population;
        }
        sort (array.begin (), array.end ());
        Simple_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                            min.at (num_countr).population, max.at (num_countr).population,
                            median.at (num_countr).population, quartile_25.at (num_countr).population,
                            quartile_75.at (num_countr).population);
        Square_dev_calculation (array, counter.at (num_countr).point_beg, counter.at (num_countr).point_end,
                                sum.population, square_deviation.at (num_countr).population,
                                average.at (num_countr).population);
        num_countr++; //переход к следующей стране
    }

    return 0;
}
