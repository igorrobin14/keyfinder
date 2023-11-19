/**
 * @file services_perso.h
 * @brief Fichier d'entete pour les fonctions permettant de creer des services personnalises
*/

#include <zephyr/types.h>

/** @brief UUID sur 128 bits du Led Button Service */
#define BT_UUID_LBS_VAL BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

/** @brief Typedef du callback de quand un changement d'etat de la LED est recu */
typedef void (*led_cb_t)(const bool led_state);

/** @brief Typedef du callback de quand le button est presse */
typedef bool (*button_cb_t)(void);

/** @brief Structure de callback utilisee par le LED Button Service. */
struct my_lbs_cb 
{
	led_cb_t led_cb;
	led_cb_t blink_cb;
	led_cb_t debug_cb;
	button_cb_t button_cb;
};

/** 
 * @brief Initialise le LED Button Service
 * @param[in] callbacks Structure contenant des pointeurs vers le fonctions de callabck utilisees par le service
*/
int my_lbs_init(struct my_lbs_cb *callbacks);

/**
 * @brief Premiere fonction permettant d'afficher la version du systeme sur le telephone
*/
int bt_set_version(void);

/**
 * @brief Deuxieme fonction permettant d'afficher la version du systeme sur le telephone
*/
static size_t read_version(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);

static void version_ccc_cfg_changed(const struct bt_gatt_attr *attr, wchar_t value);