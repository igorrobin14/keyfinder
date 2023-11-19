/**
 * @file services_perso.c
 * @brief Fichier contenant l'implementation des fonctions pour creer des services personnalises
*/

/* Inclusion des bibliothques de Zephyr */
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <bluetooth/services/lbs.h>

/* Inclusion des fichiers d'entete personnalises */
#include "services_perso.h"
#include "uuid_persos.h"

/**
 * @brief Declaration de l'etat du bouton
*/
static bool button_state;

/**
 * @brief Declaration du callback du LED Button Service
*/
static struct my_lbs_cb lbs_cb;

/**
 * @brief Declaration et initilisation de la version fictive du systeme (4.2)
*/
static wchar_t version = "v4.2";

static bool notify_enabled;
static void lbslc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_enabled = (value == BT_GATT_CCC_NOTIFY);
}

/**
 * @brief Fonction permettant l'allumage de la LED
*/
static ssize_t my_write_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
	if (len != 1U) 
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	if (offset != 0) 
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	if (lbs_cb.led_cb) 
	{
		uint8_t val = *((uint8_t *)buf);
		if (val == 0x00 || val == 0x01) 
		{
			lbs_cb.led_cb(val ? true : false);
		} 
		else 
		{
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}
	return len;
}


/**
 * @brief Fonction permettant le clignotement de la LED
*/
static ssize_t my_blink_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
	if (len != 1U) 
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	if (offset != 0) 
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	if (lbs_cb.blink_cb) 
	{
		uint8_t val = *((uint8_t *)buf);
		if (val == 0x00 || val == 0x01) 
		{
			lbs_cb.blink_cb(val ? true : false);
		} 
		else 
		{
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}
	return len;
}

static ssize_t my_set_debug(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
	if (len != 1U) 
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	if (offset != 0) 
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	if (lbs_cb.debug_cb) 
	{
		uint8_t val = *((uint8_t *)buf);
		if (val == 0x00 || val == 0x01) 
		{
			lbs_cb.debug_cb(val ? true : false);
		} 
		else 
		{
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}
	return len;
}

/**
 * @brief Fonction permettant de lire la version du systeme sur le telephone
*/
static size_t read_version(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	uint8_t version_16 = version;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &version_16, sizeof(version_16));
}

/**
 * @brief Fonction permettant d'envouer une valeur sur le telephone quand le bouton 1 est presse
*/
static ssize_t read_button(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;
	if (lbs_cb.button_cb) 
	{
		button_state = lbs_cb.button_cb();
		return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
	}

	return 0;
}

/**
 * @brief Service Alert
*/
BT_GATT_SERVICE_DEFINE(alert_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_ALERT),
/* Caracteristique LED */
BT_GATT_CHARACTERISTIC(BT_UUID_LED, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, my_write_led, NULL),
BT_GATT_CCC(lbslc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
/* Caracteristique Buzzer */
BT_GATT_CHARACTERISTIC(BT_UUID_BUZZER, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, my_blink_led, NULL),
BT_GATT_CCC(lbslc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/**
 * @brief Service System
*/
BT_GATT_SERVICE_DEFINE(system_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_SYSTEM),
/* Caracteristique Version */
BT_GATT_CHARACTERISTIC(BT_UUID_VERSION, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_version, NULL, &version),
BT_GATT_CCC(lbslc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/**
 * @brief Service Interact
*/
BT_GATT_SERVICE_DEFINE(interact_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_INTERACT),
/* Caracteristique Button */
BT_GATT_CHARACTERISTIC(BT_UUID_BUTTON, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_button, NULL, &button_state),
BT_GATT_CCC(lbslc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/**
 * @brief Service Development
*/
BT_GATT_SERVICE_DEFINE(development_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_DEVELOPMENT),
/* Caracteristique Debug */
BT_GATT_CHARACTERISTIC(BT_UUID_DEBUG, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, my_set_debug, NULL),
BT_GATT_CCC(lbslc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/** @brief Fonction permettant a tous les services d'apparaitre sur le telephone */
int my_lbs_send_button_state(bool button_state)
{
	if (!notify_enabled) {
		return -EACCES;
	}

	int err_code =  bt_gatt_notify(NULL, &system_svc.attrs[2], &button_state, sizeof(button_state))
	|| bt_gatt_notify(NULL, &interact_svc.attrs[2], &button_state, sizeof(button_state))
	|| bt_gatt_notify(NULL, &development_svc.attrs[2], &button_state, sizeof(button_state))
	|| bt_gatt_notify(NULL, &alert_svc.attrs[2], &button_state, sizeof(button_state));
				
	return err_code;
}

/**
 *	@brief Fonction initialisant tous les callbacks
*/
int my_lbs_init(struct my_lbs_cb *callbacks)
{
	if (callbacks) 
	{
		lbs_cb.led_cb = callbacks->led_cb;
		lbs_cb.button_cb = callbacks->button_cb;
		lbs_cb.blink_cb = callbacks->blink_cb;
		lbs_cb.debug_cb = callbacks->debug_cb;
	}

	return 0;
}

/**
 * @brief Fonction permettant d'afficher la version du systeme sur le telephone
*/
int bt_set_version(void)
{
	int rc = bt_gatt_notify(NULL, &system_svc.attrs[1], version, sizeof(version));

	return rc == -ENOTCONN ? 0 : rc;
}

/**
 * @brief Fonction permettant d'envoyer une valeur vers le telephone
*/
int bt_set_number(uint8_t *nb)
{
	int rc = bt_gatt_notify(NULL, &interact_svc.attrs[1], nb, sizeof(nb));
}