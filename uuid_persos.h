/**
 * @file uuid_persos.h
 * @brief Fichier contenant des UUIDs personnels
*/

#include <stdint.h>
#include <zephyr/sys/util.h>

/**
 *  @brief UUID du service Alert
 */
#define BT_UUID_ALERT_VAL 0x4201
#define BT_UUID_ALERT \
	BT_UUID_DECLARE_16(BT_UUID_ALERT_VAL)
/**
 *  @brief UUID de la caracteristique LED
 */
#define BT_UUID_LED_VAL 0x4202
#define BT_UUID_LED \
	BT_UUID_DECLARE_16(BT_UUID_LED_VAL)
/**
 *  @brief UUID de la caracteristque Buzzer
 */
#define BT_UUID_BUZZER_VAL 0x4203
#define BT_UUID_BUZZER \
	BT_UUID_DECLARE_16(BT_UUID_BUZZER_VAL)
/**
 *  @brief UUID du service System
 */
#define BT_UUID_SYSTEM_VAL 0x4206
#define BT_UUID_SYSTEM \
	BT_UUID_DECLARE_16(BT_UUID_SYSTEM_VAL)
/**
 *  @brief UUID de la caracteristique Version
 */
#define BT_UUID_VERSION_VAL 0x4207
#define BT_UUID_VERSION \
	BT_UUID_DECLARE_16(BT_UUID_VERSION_VAL)
/**
 *  @brief UUID du service Interact
 */
#define BT_UUID_INTERACT_VAL 0x4208
#define BT_UUID_INTERACT \
	BT_UUID_DECLARE_16(BT_UUID_INTERACT_VAL)
/**
 *  @brief UUID de la caracteristique Button
 */
#define BT_UUID_BUTTON_VAL 0x4209
#define BT_UUID_BUTTON \
	BT_UUID_DECLARE_16(BT_UUID_BUTTON_VAL)
/**
 *  @brief UUID du service Development
 */
#define BT_UUID_DEVELOPMENT_VAL 0x4210
#define BT_UUID_DEVELOPMENT \
	BT_UUID_DECLARE_16(BT_UUID_DEVELOPMENT_VAL)
/**
 *  @brief UUID de la caracteristique Debug
 */
#define BT_UUID_DEBUG_VAL 0x4211
#define BT_UUID_DEBUG \
	BT_UUID_DECLARE_16(BT_UUID_DEBUG_VAL)
