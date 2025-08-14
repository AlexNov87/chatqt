/**
 * @file formadmin.h
 * @brief Определение базового класса Formadmin и его наследников.
 *
 * Этот файл содержит объявления классов для управления администрированием:
 * Formadmin (базовый абстрактный класс), AdminUserForm (для клиентской стороны),
 * AnswerAdminSession (для обработки ответов) и AdminServerForm (для серверной стороны).
 *
 * @date 2025-08-13
 */

#ifndef FORMADMIN_H
#define FORMADMIN_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTcpSocket>
#include "structs.h"
#include "helpfoo.h"

class ClientWin;
class GraphicsServer;

namespace Ui {
class Formadmin;
}

/**
 * @class Formadmin
 * @brief Базовый абстрактный класс для форм администратора.
 *
 * Этот класс предоставляет общий интерфейс для управления пользователями и комнатами.
 * Он содержит слоты для обработки событий UI и виртуальные функции,
 * которые должны быть переопределены в классах-наследниках.
 */
class Formadmin : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор Formadmin.
     * @param parent Указатель на родительский виджет.
     */
    explicit Formadmin(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса Formadmin.
     */
   virtual ~Formadmin();

private slots:
    /**
     * @brief Слот, вызываемый при двойном клике по элементу списка комнат.
     * @param item Указатель на элемент списка, по которому был сделан двойной клик.
     */
    void on_lw_rooms_roomlist_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief Слот, вызываемый при клике по ячейке таблицы пользователей.
     * @param row Номер строки.
     * @param column Номер столбца.
     */
    void on_table_users_cellClicked(int row, int column);

    /**
     * @brief Слот, вызываемый при клике по вкладке на панели администрирования.
     * @param index Индекс вкладки.
     */
    void on_tw_adminpanel_tabBarClicked(int index);

    /**
     * @brief Слот, вызываемый при клике по ячейке таблицы комнат.
     * @param row Номер строки.
     * @param column Номер столбца.
     */
    void on_table_rooms_cellClicked(int row, int column);

protected:
    Ui::Formadmin *ui; /**< Указатель на объект UI-формы. */
    const str_type _EMPTY_ROOM_LBL = "Press on roomlist to select room"; /**< Строка-заглушка для пустого выбора комнаты. */

protected:
    //-------------------------------------------------------------------------
    // Виртуальные функции для управления комнатами
    //-------------------------------------------------------------------------

    /**
     * @brief Виртуальная функция для обработки удаления комнаты.
     */
    virtual void OnDeleteRoomClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки создания комнаты.
     */
    virtual void OnCreateRoomClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки обновления комнаты.
     */
    virtual void OnUpdateRoomClicked() = 0;

protected:
    //-------------------------------------------------------------------------
    // Виртуальные функции для управления пользователями
    //-------------------------------------------------------------------------

    /**
     * @brief Виртуальная функция для обработки блокировки пользователя.
     */
    virtual void OnBlockUserClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки разблокировки пользователя.
     */
    virtual void OnUnblockUserClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки удаления пользователя.
     */
    virtual void OnDeleteUserClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки изменения роли пользователя.
     */
    virtual void OnModifyUserRoleClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки обновления списка пользователей.
     */
    virtual void OnUpdateUsersClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки поиска пользователей.
     */
    virtual void OnFindUsersClicked() = 0;

    /**
     * @brief Виртуальная функция для обработки получения всех пользователей.
     */
    virtual void OnGetAllUsersClicked() = 0;

protected:
    //-------------------------------------------------------------------------
    // Виртуальные функции для обработки выбора вкладок
    //-------------------------------------------------------------------------

    /**
     * @brief Виртуальная функция, вызываемая при выборе вкладки "Пользователи".
     */
    virtual void TabUsersSelected() = 0;

    /**
     * @brief Виртуальная функция, вызываемая при выборе вкладки "Комнаты".
     */
    virtual void TabRoomsSelected() = 0;
};

// ---

/**
 * @class AdminUserForm
 * @brief Класс, реализующий форму администрирования для клиентского приложения.
 *
 * Этот класс наследуется от Formadmin и предоставляет конкретную
 * реализацию методов для управления комнатами и пользователями,
 * взаимодействуя с серверной частью через сокет.
 */
class AdminUserForm : public Formadmin {
public:
    /**
     * @brief Конструктор AdminUserForm.
     * @param client Указатель на окно клиента.
     */
    AdminUserForm(ClientWin* client);

private:
    /**
     * @brief Инициализирует форму и её компоненты.
     */
    void Init();

    void OnDeleteRoomClicked() override;
    void OnCreateRoomClicked() override;
    void OnUpdateRoomClicked() override;

private:
    void OnBlockUserClicked() override;
    void OnUnblockUserClicked() override;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override;
    void OnUpdateUsersClicked() override;
    void OnFindUsersClicked() override;
    void OnGetAllUsersClicked() override;

private:
    void TabUsersSelected() override {}
    void TabRoomsSelected() override {}

private:
    /**
     * @brief Обрабатывает сокет-соединение для административных задач.
     */
    void HaldleSocket();

    /**
     * @brief Заполняет таблицу комнат.
     * @param rooms_json JSON-объект с данными о комнатах.
     */
    void FillRoomTable(const);

    /**
     * @brief Заполняет таблицу пользователей.
     */
    void FillUserTable();

    friend class AnswerAdminSession; /**< Класс, имеющий доступ к приватным членам. */
    ClientWin* _clientwin; /**< Указатель на родительское окно клиента. */
    SocketComplect _sock; /**< Объект для управления сокет-соединением. */
    std::shared_ptr<QTcpSocket> _socket_for_admin; /**< Указатель на сокет. */
    QHostAddress _adrs; /**< Адрес хоста. */
};

// ---

/**
 * @class AnswerAdminSession
 * @brief Класс для обработки ответов сервера в административной сессии.
 *
 * Этот класс предназначен для парсинга и выполнения действий
 * на основе JSON-ответов, полученных от сервера.
 */
class AnswerAdminSession
{
public:
    /**
     * @brief Конструктор AnswerAdminSession.
     * @param uform Указатель на родительскую форму администратора.
     * @param obj JSON-объект ответа сервера.
     */
    AnswerAdminSession(AdminUserForm* uform, const json_obj& obj);

    /**
     * @brief Начинает выполнение действий на основе JSON-ответа.
     */
    void StartExecute();

private:
    AdminUserForm* _user_form; /**< Указатель на форму администратора. */
    const json_obj& _obj; /**< Ссылка на JSON-объект ответа. */

    /**
     * @brief Получает тип действия из JSON-объекта.
     * @return Тип действия или std::nullopt, если действие не найдено.
     */
    std::optional<ADMIN_ACTIONS> GetAction();

    /**
     * @brief Выполняет действия для списка комнат.
     */
    void ExecuteRoomList();

    /**
     * @brief Выполняет действия для списка пользователей.
     */
    void ExecuteUserslist();

    /**
     * @brief Выполняет действия для добавления комнаты.
     */
    void ExecuteAddRoom();

    /**
     * @brief Выполняет действия для удаления комнаты.
     */
    void ExecuteDeleteRoom();

    /**
     * @brief Выполняет действия для удаления пользователя.
     */
    void ExecuteDeleteUser();

    /**
     * @brief Выполняет действия для блокировки пользователя.
     */
    void ExecuteBanUser();

    /**
     * @brief Выполняет действия для разблокировки пользователя.
     */
    void ExecuteUnbanUser();

    /**
     * @brief Выполняет действия для обновления роли пользователя.
     */
    void ExecuteUpdateRole();

    /**
     * @brief Выполняет системные действия.
     */
    void ExecuteSystem();
};

// ---

/**
 * @class AdminServerForm
 * @brief Класс, реализующий форму администрирования для серверного приложения.
 *
 * Этот класс наследуется от Formadmin и предоставляет конкретную
 * реализацию методов для управления комнатами и пользователями
 * непосредственно на сервере.
 */
class AdminServerForm : public Formadmin {
public:
    /**
     * @brief Конструктор AdminServerForm.
     * @param srv Указатель на серверный объект GraphicsServer.
     */
    AdminServerForm(GraphicsServer* srv);

    /**
     * @brief Инициализирует форму и её компоненты.
     */
    void Init();

private:
    void OnDeleteRoomClicked() override;
    void OnCreateRoomClicked() override;
    void OnUpdateRoomClicked() override;

private:
    void OnBlockUserClicked() override;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override;
    void OnUpdateUsersClicked() override;
    void OnUnblockUserClicked() override;
    void OnFindUsersClicked() override;
    void OnGetAllUsersClicked() override;

private:
    void TabUsersSelected() override {}
    void TabRoomsSelected() override;

    /**
     * @brief Внутренняя функция для обновления списка комнат.
     */
    void UpdateRoomsInternal();

    GraphicsServer* _srv; /**< Указатель на сервер. */
};

#endif // FORMADMIN_H
