/**
 * @file main.c
 * @brief Fichier contenant le programme principal
*/

/* Inclusion des bibliotheques de Zephyr */
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/types.h>
#include <stdio.h>
#include <dk_buttons_and_leds.h>
#include "nrf_gpio.h"

/* Inclusion des bibliotheques personnelles */
#include "services_perso.h"
#include "uuid_persos.h"

/* Nom de l'appareil */
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* Definition des bouton et LEDs utilises */

/* Bouton pour l'envoi de l'informatio sur le telephone (bouton 1)*/
#define BUTTON_1_PIN 13

/* LED pour l'allumage de la LED (service Alert caracteristique LED)*/
#define USER_LED DK_LED3

/* LED pour le buzzer (clignotement, service Alert caracteristique Buzzer)*/
#define BLINK_LED DK_LED2

/* LED allumee pour signaler que le keyfinder est en mode Debug (LED 1, service Development caracteristique Debug)*/
#define DEBUG_LED DK_LED1

#define USER_BUTTON DK_BTN1_MSK

/* Definition du temps d'attente entre 2 boucles principales while */
#define WAIT_TIME 1000

/* Definition du temps d'attente entre l'allumage et l'eteignage de la LED simulant le buzzer */
#define BLINK_TIME 500

/* Etat du bouton pour l'envoie d'information sur le telephone */
static bool app_button_state;

/* Mode debug actif ou non (non a la base) */
static bool debug_mode_on = false;

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY), 800, 801,NULL); 

uint8_t nb = 42;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_LBS_VAL),
};

/**
 * @brief Fonction de callback pour l'allumage de LED
*/
static void app_led_cb(bool led_state)
{
	dk_set_led(USER_LED, led_state);
	if (debug_mode_on == true)
	{
		printk("Fonction de callback pour l'allumage de LED\n");
	}
}

/**
 * @brief Fonction de callback pour le clignotement de LED
*/
static void app_blink_cb(bool led_state)
{
	for (int i = 0; i < 20; i++)
	{
		dk_set_led(BLINK_LED, led_state);
		k_sleep(K_MSEC(BLINK_TIME));
		if (debug_mode_on == true)
		{
			printk("LED allumee\n");
		}
		dk_set_led(BLINK_LED, !led_state);
		k_sleep(K_MSEC(BLINK_TIME));
		if (debug_mode_on == true)
		{
			printk("LED eteinte\n");
		}
	}
}

/**
 * @brief Fonction de callback pour le mode Debug
*/
static void app_debug_cb(bool led_state)
{
	dk_set_led(DEBUG_LED, led_state);

	if (led_state == true)
	{
		debug_mode_on = true;
		printk("Mode de debug actif\n");
	}
}

/**
 * @brief Fonction de callback pour lire l'etat du bouton
*/
static bool app_button_cb(void)
{
	return app_button_state;
	if (debug_mode_on == true)
	{
		printk("Fonction de callback pour l'etat du bouton\n");
	}
}

/**
 * @brief Liste des callbacks
*/
static struct my_lbs_cb app_callbacks = {
	.led_cb = app_led_cb,
	.blink_cb = app_blink_cb,
	.debug_cb = app_debug_cb,
	.button_cb = app_button_cb,
};

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	if (has_changed & USER_BUTTON) 
	{
		uint32_t user_button_state = button_state & USER_BUTTON;
		app_button_state = user_button_state ? true : false;
		if (debug_mode_on == true)
		{
			printk("L'etat du bouton change\n");
		}
	}
}

static void on_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) 
	{
		if (debug_mode_on == true)
		{
			printk("Connexion echouee (err %u)\n", err);
			return;
		}
	}
	if (debug_mode_on == true)
	{
		printk("Connecte\n");
	}
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	if (debug_mode_on == true)
	{
		printk("Deconnecte (raison %u)\n", reason);
	}
}

struct bt_conn_cb connection_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected,
};

static int init_button(void)
{
	int err;
	err = dk_buttons_init(button_changed);
	if (err) 
	{
		if (debug_mode_on == true)
		{
			printk("Impossible d'initialiser les boutons (err: %d)\n", err);
		}
	}
	return err;
}

/**
 * @brief Fonction permettant d'afficher la version du systeme sur le telephone
*/
static void version_notify(void)
{
	bt_set_version();
}

/**
 * @brief Fonction permettant d'envoyer une valeur le telephone quand le bouton est presse
*/
static void button_notify(uint8_t *nb)
{
	bt_set_number(nb);
}

void main(void)
{
	int blink_status = 0;
	int err;

	nrf_gpio_cfg_input(BUTTON_1_PIN, NRF_GPIO_PIN_PULLUP);

	err = dk_leds_init();

	if (debug_mode_on == true)
	{
		printk("Initialisation des LEDs\n");
	}

	if (err) 
	{
		return;
	}

	err = init_button();
	if (debug_mode_on == true)
	{
		printk("Initialisation des boutons\n");
	}

	if (err) 
	{
		if (debug_mode_on == true)
		{
			printk("Impossible d'initialiser les boutons (err %d)\n", err);
			return;
		}
	}

	err = bt_enable(NULL);

	if (debug_mode_on == true)
	{
		printk("Activation du Bluetooth\n");
	}

	if (err) 
	{
		return;
	}

	bt_conn_cb_register(&connection_callbacks);

	if (debug_mode_on == true)
	{
		printk("bt_conn_register()\n");
	}

	err = my_lbs_init(&app_callbacks);

	if (debug_mode_on == true)
	{
		printk("Initialisation du LBS\n");
	}

	if (err) 
	{
		if (debug_mode_on == true)
		{
			printk("Impossible d'initilaiser le LED Button Service (err:%d)\n", err);
			return;
		}
	}
	err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

	if (debug_mode_on == true)
	{
		printk("Demarrage de l'avertising\n");
	}

	if (err) 
	{
		return;
	}

	while(1) 
	{
		k_sleep(K_MSEC(WAIT_TIME));

		version_notify();

		if (debug_mode_on == true)
		{
			printk("Envoi de la version du systeme\n");
		}

		if (nrf_gpio_pin_read(BUTTON_1_PIN) == 0)
		{
			button_notify(&nb);
			if (debug_mode_on == true)
			{
				printk("Envoi de l'information apres appui sur bouton\n");
			}
			nb++;
		}
	}
}