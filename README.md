# Séances 3 et 4 de Design : programmation sur la carte nRF52 DK de Nordic Semiconductors - Compte-rendu

## Igor Robin - ZZ2 F1 - Semestre 1

### Contenu du dépôt
- ``` main.c ``` - Fichier contenant le **programme principal** de mise en œuvre de services personnalisés afin du simuler le comportement d'un keyfinder de chez Action :
    - Allumage d'une LED, 
    - Déclenchement d'un bip sonore, 
    - Envoi d'informations sur le téléphone
    - Lecture de la version fictive du système sur le téléphone
    - Passage en mode Debug

###
 - ``` services_perso.c ``` - Fichier contenant l'implémentation des **fonctions** permettant d'obtenir des services personnalisés sur l'application nRF Connect
 - ``` services_perso.h ``` - Fichier d'entête permettant de déclarer les **prototypes** de fonctions présentes dans services_perso.c ainsi que des typedefs de fonctions

###
 - ``` uuid_persos.h ``` - Fichier contenant la définition des **UUIDs** personnalisés commençant par 0x42
 - ``` nrf_gpio.h ``` - Fichier issu d'un exemple de **programme fourni avec la suite Nordic** permettant l'utilisation des différents GPIOs

 - ``` prj.conf ``` - Fichier de **configuration générale** du keyfinder simulé
 - ``` CMakeLists.txt ``` - Fichier CMake légèrement modifié par rapport à celui lors de l'initialisation d'une nouvelle application via nRF Connect pour y rajouter le fichier ``` services_perso.c ```
 - Ce ``` README.md ``` 

###
 - ``` tableau_services_caracteristiques.xlsx ``` - Tableau Excel des **services et caractéristiques** personnalisés implémentés

###
*Utilisation : mettre tous les fichiers dans le* ```src``` *de l'application nRF Connect sur Visual Studio Code une fois qu'elle à été créée*

## Introduction
Le but de ces deux dernières séances de Design était de simuler un **keyfinder** de chez Action grâce à la carte **nRF52 DK** de Nordic Semiconductors en le dotant de propriétés telles que **l'allumage d'une LED** au moment désiré via le téléphone, le **déclenchement d'un buzzer** (simulé ici par le clignotement d'une LED 20 fois), la lecture de la **version** fictive du système (v4.2), **l'envoi d'information** sur le téléphone quand un bouton est appuyé sur la carte et enfin le passage en mode **Debug** depuis le téléphone.

Cette simulation de keyfinder a été réalisée en s'inspirant de fonctions du **LED Button Service** (LBS) déjà présent dans l'exemple de code **Bluetooth : Peripheral LBS**, de fonctions tirées d'un **exercice-tutoriel** sur le site de **Nordic Dev Academy** et de fonctions expliquées dans une page web du site appelé **Embedded Explorer**.
 - **Bluetooth - Peripheral LBS** : 
 https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/bluetooth/peripheral_lbs/README.html#peripheral-lbs
 - **Exercice-tutoriel sur Nordic Dev Academy** : 
 https://github.com/NordicDeveloperAcademy/bt-fund/tree/main/lesson4/blefund_less4_exer1_solution/src
 - **Page web du site Embedded Explorer** : 
 https://embeddedexplorer.com/nrf52-gpio-tutorial/

Ainsi, des morceaux de code ont été **repris** de ces sources, **modifiés** et **complétés** afin de satisfaire les exigences requises.

## Ce qui a été mis en œuvre
 - Création **d'UUIDs personnalisés** basés sur le nombre 42 (ex : ```0x4201```, ```0x4210```, etc)
 - Affichage de **services et de caractéristiques personnalisés** sur le téléphone une fois qu'il est connecté à la carte ayant pour UUIDs ces UUIDs personnalisés
 - Affichage d'un **nom personnalisé** pour le keyfinder lorsque le téléphone est connecté à celui-ci (Keyfinder de Action Igor)
 - **Allumage d'une LED** lors de l'envoi d'un ```true``` (n'importe quelle valeur supérieure à 0) **depuis le téléphone**
 - Déclenchement d'un **buzzer** lors de l'envoi d'un ```true``` depuis le téléphone (simulé ici par le **cligonetement d'une LED** 20 fois à une fréquence de 1 Hz)
 - Lecture de la **version** fictive du système présent sur le keyfinder (v4.2) sous la forme d'une **chaine de caractères**
 - Envoi d'un true **sur le téléphone depuis le keyfinder** lors de l'appui sur un **bouton présent sur le keyfinder**
 - Possibilité de passer en **mode Debug** : lorsque l'utilisateur envoie un ```true``` depuis le téléphone, des logs correspondant aux opérations en cours d'exécution s'affichent dans le nRF Connect Serial Terminal

## Ce qui a moins bien marché et qu'il faudrait modifier/compléter
 - Les services nouvellement créés n'ont **pas de noms propres**, le seul moyen d'en avoir un est de renommer les *Unknown Service* directement sur l'application en faisant une **édition de leur nom** via un appui prolongé sur ceux-ci.
 - Il est difficile d'utiliser la fonctionnalité permettant d'envoyer un ```true``` sur le téléphone depuis le keyfinder : il faut **rester appuyé sur le bouton 1 de la carte et en même temps appuyer sur la flèche descendante** en face de la caractéristiques correspondante pour que le téléphone reçoive le ```true```.
 - Il n'est actuellement pas possible d'envoyer une autre valeur qu'un ```true``` sur le téléphone (ex : le nombre 42).
 - La chaine de caractères lue pour la version du système n'est **pas affichée correctement** car elle semble être précédée de sa conversion via la table ASCII.
 - Il faudrait brancher un **haut-parleur** à la carte pour mettre en œuvre le buzzer sans le simuler avec une LED.

## Informations utiles
 - Afin de s'y **retrouver**, bien penser à **renommer** les **services et caractéristiques** sur l'application nRF Connect en se basant sur leurs **UUIDs** présents dans le fichier ```tableau_services_caractertistiques.xlsx```.
 - Afin de voir les **logs** pour le mode Debug, il faut ouvrir le **Serial Terminal** dans **nRF Connect for Desktop**, sélectionner la carte et le **Serial Port COM5**, après avoir bien évidemment activé le mode Debug en allant dans le **service Development, dans la caractéristique Debug** et en ayant envoyé un ```true```.
 - Afin de lire la **version** du système sur le keyfinder sur le téléphone, il faut aller dans le **service System**, puis dans la **caractéristique Version**, appuyer sur **flèche descendante puis sur les 3 flèches descendantes**.
 - Afin d'envoyer des ```true```, appuyer lorsque cela est proposé sur la **flèche montante** et faire un send d'un **UINT8 de valeur strictement supérieure à 0**.

## Détail de la mise en œuvre
 - Exemple de déclaration d'UUID de service et de caractéristique personnalisé :
 ```c
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
 ```
 - Exemple de déclaration d'un **service et de ses caractéristiques associées** (on en déclare autant que nécessaire), en précisant à chaque fois le **nom du service** (ici ```alert_svc```), ses **permissions** de lecture, d'écriture ou de notification, les **fonctions associées** permettant la gestion ici des LEDs et en ajoutant le **CCC**.
 ```c
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
```
- On modifie la fonction ```int my_lbs_send_button_state(bool button_state)``` afin qu'elle permette **l'affichage de tous les services** sur le téléphone (OU logique entre tous les appels à la fonction ```bt_gatt_notify```). De plus, on appelle cette même fonction en lui passant en paramètre ```nom_du_service.attrs[2]```.
```c
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
```
- On déclare et on initialise les **callbacks** dont on a besoin en modifiant la fonction de base.
```c
/**
 * @brief Liste des callbacks
*/
static struct my_lbs_cb app_callbacks = {
	.led_cb = app_led_cb,
	.blink_cb = app_blink_cb,
	.debug_cb = app_debug_cb,
	.button_cb = app_button_cb,
};

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
```
- On implémente des fonctions de callback en s'inspirant de la fonction ```static void app_button_cb(void)```
- Pour ```app_blink_cb()```, on peut voir qu'on a fait une boucle ```for``` sur **20 itérations** où on allume puis on éteint la LED censée clignoter (```BLINK_LED```) afin de **simuler le buzzer**, on peut noter que pour faire **clignoter la LED**, il suffit de faire un ```dk_set_led()``` avec le ```led_state``` opposé (```!led_state```).
- Pour ```app_debug_cb()```, on peut voir que si la LED de debug est allumée, on active fait passer ```debug_mode_on``` à ```true```, qui est une variable globale déclarée au début du fichier ```main.c``` Si ce mode est activé, on affiche toutes les actions effectuées par le programme dans le Serial Terminal de nRF Connect.
```c
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
```
- Ici, on peut voir qu'on crée des fonctions permettant **d'envoyer de l'information** sur le téléphone appelant des fonctions ```bt_set()``` qui vont appeler la fonction ```bt_gatt_ontify()``` avec le ```&[service]_svc.attrs[1]``` correspondant, **l'information à transmettre et sa taille**.
```c
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
```
- La **boucle** ```while``` **principale** consiste à attendre un **délai imposé**, à envoyer l'information de la version **sur le téléphone**, à lire si le bouton 1 est pressé (dans ce cas on envoie un ```true``` sur le téléphone). Ceci est complété par des **messages sur le terminal** si le mode de debug est **activé**.
```c
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
```
- Les fonctions ```my_blink_led()```, ```my_write_led()```, ```read_version()``` et ```read_button()``` sont inspirées de la fonction ```write_led()``` présente dans le Peripheral LBS et sont **présentes dans la déclaration des services** - Exemple de fonction :
```c
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
/**
 * @brief Fonction permettant de lire la version du systeme sur le telephone
*/
static size_t read_version(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	uint8_t version_16 = version;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &version_16, sizeof(version_16));
}
```
- Toutes les **autres fonctions, variables et macros** sont tirées de l'exemple du **Peripheral LBS** (```adv_param```, ```ad```, ```sd```, ```on connected()```, ```on disconnected()```, ```init_button()```, ```bt_conn_cb_register()```, ```bt_le_adv_start()```, etc) et permettent d'assurer le **fonctionnement, la communication, l'initialisation** du keyfinder.

## Conclusion
Durant ces deux séances de Design, la **simulation d'un keyfinder de chez Action** sur la carte nRF52 DK de Nordic Semiconductors a été dans l'ensemble un **succès**, puisque les exigences de fonctionnalités requises ont été satisfaites. Néanmoins, il reste des points à améliorer si l'on souhaite se rapprocher d'une simulation encore plus réaliste ainsi que des aspects du code à comprendre et à maîtriser.




