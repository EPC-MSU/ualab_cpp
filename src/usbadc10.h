/**
 * @file usbadc10.h
 * @brief usbadc10 API
 */
/* Project generated by builder 0.10.9 */
#ifndef INC_USBADC10_H
#define INC_USBADC10_H
#define USBADC10_BUILDER_VERSION_MAJOR 0
#define USBADC10_BUILDER_VERSION_MINOR 10
#define USBADC10_BUILDER_VERSION_BUGFIX 9
#define USBADC10_BUILDER_VERSION_SUFFIX ""
#define USBADC10_BUILDER_VERSION "0.10.9"


#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <wchar.h>


#undef USBADC10_URPC_API_EXPORT
#if defined(_WIN32)
    #if USBADC10_URPC_BUILDING_STAGE
        #define USBADC10_URPC_API_EXPORT __declspec(dllexport)
    #else
        #define USBADC10_URPC_API_EXPORT __declspec(dllimport)
    #endif
#else
    #define USBADC10_URPC_API_EXPORT __attribute__((visibility("default")))
#endif

#undef USBADC10_URPC_CALLING_CONVENTION
#if defined(_WIN32)
    #define USBADC10_URPC_CALLING_CONVENTION __cdecl
#else
    #define USBADC10_URPC_CALLING_CONVENTION
#endif

typedef int device_t;
#define device_undefined (-1)
typedef int result_t;
#define result_ok 0
#define result_error (-1)
#define result_not_implemented (-2)
#define result_value_error (-3)
#define result_nodevice (-4)
/**
 * \~english
 * @name Logging level
 * \~russian
 * @name Уровень логирования
 */
//@{
/**
 * \~english
 * Logging level - error
 * \~russian
 * Уровень логирования - ошибка
 */
#define LOGLEVEL_ERROR 		0x01
/**
 * \~english
 * Logging level - warning
 * \~russian
 * Уровень логирования - предупреждение
 */
#define LOGLEVEL_WARNING 	0x02
/**
 * \~english
 * Logging level - info
 * \~russian
 * Уровень логирования - информация
 */
#define LOGLEVEL_INFO		0x03
/**
 * \~english
 * Logging level - debug
 * \~russian
 * Уровень логирования - отладка
 */
#define LOGLEVEL_DEBUG		0x04
//@}

/**
 * \~english
 * Logging callback prototype.
 * @param loglevel - A logging level.
 * @param message - A message.
 * \~russian
 * Прототип функции обратного вызова для логирования.
 * @param loglevel - Уровень логирования.
 * @param message - Сообщение.
 */
typedef void (USBADC10_URPC_CALLING_CONVENTION *usbadc10_logging_callback_t)(int loglevel, const wchar_t *message, void *user_data);

/**
 * \~english
 * Simple callback for logging to stderr in wide chars.
 * @param loglevel - A logging level.
 * @param message - A message.
 * \~russian
 * Простая функция логирования на stderr в широких символах.
 * @param loglevel - Уровень логирования.
 * @param message - Сообщение.
 */
USBADC10_URPC_API_EXPORT void USBADC10_URPC_CALLING_CONVENTION usbadc10_logging_callback_stderr_wide(int loglevel, const wchar_t *message, void *user_data);

/**
 * \~english
 * Simple callback for logging to stderr in narrow (single byte) chars.
 * @param loglevel - A logging level.
 * @param message - A message.
 * \~russian
 * Простая функция логирования на stderr в узких (однобайтных) символах.
 * @param loglevel - Уровень логирования.
 * @param message - Сообщение.
 */
USBADC10_URPC_API_EXPORT void USBADC10_URPC_CALLING_CONVENTION usbadc10_logging_callback_stderr_narrow(int loglevel, const wchar_t *message, void *user_data);

/**
 * \~english
 * Sets a logging callback.
 * Passing NULL disables logging.
 * @param logging_callback a callback for log messages
 * \~russian
 * Устанавливает функцию обратного вызова для логирования.
 * Передача NULL в качестве аргумента отключает логирование.
 * @param logging_callback указатель на функцию обратного вызова
 */
USBADC10_URPC_API_EXPORT void USBADC10_URPC_CALLING_CONVENTION usbadc10_set_logging_callback(usbadc10_logging_callback_t cb, void *data);
typedef struct
{
    uint8_t Manufacturer[16]; /**< \~english Manufacturer name. The name is set by the manufacturer. \~russian Имя производителя. Устанавливается производителем. */
    uint8_t ProductName[16]; /**< \~english Product name. The name is set by the manufacturer. \~russian Название продукта. Устанавливается производителем. */
    uint8_t ControllerName[16]; /**< \~english User controller name. This name can be set by user via additional command. \~russian Пользовательское имя контроллера. Может быть установлено пользователем с помощью отдельной команды. */
    uint8_t HardwareMajor; /**< \~english Major number of the hardware version. \~russian Основной номер версии железа. */
    uint8_t HardwareMinor; /**< \~english Minor number of the hardware version. \~russian Второстепенный номер версии железа. */
    uint16_t HardwareBugfix; /**< \~english Revision number of the hardware version. \~russian Номер ревизии платы. */
    uint8_t BootloaderMajor; /**< \~english Bootloader major version number. \~russian Мажорный номер версии загрузчика. */
    uint8_t BootloaderMinor; /**< \~english Bootloader minor version number. \~russian Минорный номер версии загрузчика. */
    uint16_t BootloaderBugfix; /**< \~english Bootloader revision number. \~russian Номер ревизии загрузчика. */
    uint8_t FirmwareMajor; /**< \~english Firmware major version number. \~russian Мажорный номер версии прошивки. */
    uint8_t FirmwareMinor; /**< \~english Firmware minor version number. \~russian Минорный номер версии прошивки. */
    uint16_t FirmwareBugfix; /**< \~english Firmware revision number. \~russian Номер ревизии прошивки. */
    uint32_t SerialNumber; /**< \~english Device serial number. \~russian Серийный номер изделия. */
    uint8_t Reserved[8]; /**< \~english Software should not rely on the value of this field. To provide compatibility with future products the value of this field shouldn’t be modified. \~russian Значение данного поля не должно использоваться в прикладном ПО. Для обеспечения совместимости с другими устройствами не изменяйте значение этого поля. */

} usbadc10_get_identity_information_t;

typedef struct
{
    uint16_t data[10]; /**< \~english Array with conversion results from 10 ADC channels with 12-bit precision. 0 corresponds to minimal voltage (GND), 4095 corresponds to ADC reference voltage (3.3 V). \~russian Массив результатов измерений с 10 каналов 12-битного АЦП. 0 соответствует минимальному напряжению (GND), 4095 соответствует опорному напряжению АЦП (3,3 В). */

} usbadc10_get_conversion_raw_t;

typedef struct
{
    uint16_t data[10]; /**< \~english Array with conversion results from 10 channels. Units: 100 uV. For example returned value 123 corresponds to the voltage of 12.3 mV. \~russian Массив результатов измерений с 10 каналов. Единицы измерения: 100 мкВ. Например, полученное значение 123 соответствует напряжению 12,3 мВ. */

} usbadc10_get_conversion_t;
/**
 * \~english
 * Open a device by name \a name and return identifier of the device which can be used in calls.
 * @param[in] name - A device name.
 * Device name has form "com:port" or "xi-net://host/serial".
 * In case of USB-COM port the "port" is the OS device uri.
 * For example "com:\\.\COM3" in Windows or "com:///dev/ttyACM34" in Linux/Mac.
 * In case of network device the "host" is an IPv4 address or fully qualified domain uri (FQDN),
 * "serial" is the device serial number in hexadecimal system.
 * For example "xi-net://192.168.0.1/00001234" or "xi-net://hostname.com/89ABCDEF".
 * Note: only one program may use COM-device in same time.
 * If errors occur when opening device, you need to make sure that the COM port is in the system and
 * device is not currently used by other programs.
 * \~russian
 * Открывает устройство по имени \a name и возвращает идентификатор устройства.
 * @param[in] name - Имя устройства.
 * Имя устройства имеет вид "com:port" или xi-net://host/serial.
 * Для COM устройства "port" это имя устройства в ОС.
 * Например "com:\\.\COM3" (Windows) или "com:///dev/tty/ttyACM34" (Linux/Mac).
 * Для сетевого устройства "host" это IPv4 адрес или полностью определённое имя домена,
 * "serial" это серийный номер устройства в шестнадцатеричной системе.
 * Например "xi-net://192.168.0.1/00001234" или "xi-net://hostname.com/89ABCDEF".
 * Замечание: в один момент времени COM устройство может использоваться только одной программой.
 * Если при открытии устройства возникают ошибки, нужно убедиться, что COM-порт есть в системе и что это
 * устройство в данный момент не используется другими программами
 */
USBADC10_URPC_API_EXPORT device_t USBADC10_URPC_CALLING_CONVENTION usbadc10_open_device(const char *uri);

/**
 * \~english
 * Get library version.
 * @param[out] lib_version - Library version.
 * \~russian
 * Версия библиотеки.
 * @param[out] lib_version - Версия библиотеки.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_libversion(char *lib_version);

/**
 * \~english
 * Return device identity information such as firmware version and serial number. It is useful to find your device in a list of available devices. It can be called from the firmware and bootloader.
 * @param[in] handle - Device ID, obtained by usbadc10_open_device() function.
 * @param[out] output - Device out data.
 * \~russian
 * Возвращает идентификационную информацию об устройстве, такую как номера версий прошивки и серийный номер. Эта информация удобна для поиска нужного устройства среди списка доступных. Может быть вызвана как из прошивки, так и из бутлоадера.
 * @param[in] handle - Идентификатор устройства, полученный от usbadc10_open_device().
 * @param[out] output - Данные, получаемые с устройства.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_get_identity_information(device_t handle, usbadc10_get_identity_information_t *output);

/**
 * \~english
 * Gets the last conversion result from all channels in ADC codes.
 * @param[in] handle - Device ID, obtained by usbadc10_open_device() function.
 * @param[out] output - Device out data.
 * \~russian
 * Получение результата последнего измерения со всех каналов в кодах АЦП.
 * @param[in] handle - Идентификатор устройства, полученный от usbadc10_open_device().
 * @param[out] output - Данные, получаемые с устройства.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_get_conversion_raw(device_t handle, usbadc10_get_conversion_raw_t *output);

/**
 * \~english
 * Gets the last conversion result from all channels.
 * @param[in] handle - Device ID, obtained by usbadc10_open_device() function.
 * @param[out] output - Device out data.
 * \~russian
 * Получение результата последнего измерения со всех каналов.
 * @param[in] handle - Идентификатор устройства, полученный от usbadc10_open_device().
 * @param[out] output - Данные, получаемые с устройства.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_get_conversion(device_t handle, usbadc10_get_conversion_t *output);

/**
 * \~english
 * Close specified device.
 * @param handle_ptr - An identifier of device.
 * \~russian
 * Закрывает устройство.
 * @param handle_ptr - Идентификатор устройства.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_close_device(device_t *handle_ptr);

/**
 * \~english
 * Load profile from device.
 * @param[in] handle - Device id.
 * @param[out] buffer - Pointer to output char* buffer. Memory for char* pointer must be allocated.
 * @param[out] allocate - Function for memory allocate.
 * \~russian
 * Загружает профиль с устройства.
 * @param[in] handle - Идентификатор устройства.
 * @param[out] buffer -  Адрес указателя на выходной буфер.
 * Память для указателя на char* должна быть выделена.
 * @param[out] allocate - Функция для выделения памяти.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_get_profile(device_t handle, char **buffer, void *(*allocate)(size_t));

/**
 * \~english
 * Save profile to device
 * @param[in] handle - Device id.
 * @param[in] buffer - Input char* buffer.
 * \~russian
 * Загружает профиль с устройства.
 * @param[in] handle - Идентификатор устройства.
 * @param[in] buffer - Входной буфер, откуда будет считан профиль.
 */
USBADC10_URPC_API_EXPORT result_t USBADC10_URPC_CALLING_CONVENTION usbadc10_set_profile(device_t handle, char *buffer);

#ifdef __cplusplus
}
#endif
#endif
