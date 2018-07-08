const defaultLanguage = "en";

const strings = {
  en: {
    NAVIGATION_ABOUT: "About",
    NAVIGATION_USAGE: "Usage",
    NAVIGATION_DOCUMENTATION: "Documentation",
    NAVIGATION_GET: "Download",
    NAVIGATION_LANGUAGE: "Language",

    ABOUT_INTRODUCTION_HEADER: "What's Backy?",
    ABOUT_INTRODUCTION_CONTENT: "<p>Backy is a simple and free open-source solution for creating and maintaining backups. It's very easy to use and it's suitable for both Linux® and Windows® operating systems.</p><p>Backy allows you to create a backup of either a folder or an entire drive. Then, whenever your source content changes you can simply update your previous backup.</p>",
    ABOUT_AUTHOR_HEADER: "Author",
    ABOUT_AUTHOR_CONTENT: "<p>This application has been developed by <a href='mailto:marek.felsoci@gmail.com'>Marek Felsoci</a>, IT student at the <a href='http://www.unistra.fr' target='_blank'>University of Strasbourg, France</a>.",
    ABOUT_CONTACT_HEADER: "Do you have questions, suggestions or want to contribute?",
    ABOUT_CONTACT_CONTENT: "<p>Please let the author know about it either by e-mail at <a href='mailto:marek.felsoci@gmail.com'>marek.felsoci@gmail.com</a> or through the project's <a href='https://github.com/felsocim/Backy' target='_blank'>GitHub® repository</a>.</p><p>Thank you!</p>",
    ABOUT_LICENSE_HEADER: "License",
    ABOUT_LICENSE_CONTENT: "<p>Backy is licensed under the terms of <a href='https://www.gnu.org/licenses/gpl-3.0.en.html' target='_blank'>GNU General Public License version 3.0</a>.</p>",
    USAGE_INTRODUCTION_HEADER: "How to use Backy?",
    USAGE_INTRODUCTION_CONTENT: "<p>Backy's graphical user interface is very intuitive notable for beginner users.</p>",
    USAGE_CREATE_BACKUP_HEADER: "Create a backup",
    USAGE_CREATE_BACKUP_CONTENT: "<p>To create a backup, you need to specify the location where the content to be backup up is stored as well as the location where the backup will be created. Then uncheck the <strong>Synchronize only</strong> checkbox in order to create a fresh backup. Finally, click on the <strong>Backup</strong> button to begin the backup process and wait until it completes.</p>",
    USAGE_UPDATE_BACKUP_HEADER: "Update/Synchronize a backup",
    USAGE_UPDATE_BACKUP_CONTENT: "<p>If you already had done a backup of the selected source location, you can synchronize it with the most recent changes of the latter without having to re-create an another backup which can become very disk space-consuming.</p><p>Backy will compare all the items in the source and the target backup locations and copy only those that had been changed since the last backup. You can choose between two comparison criterions. Either the most recent version or the biggest (in terms of file size) version of an item will be kept.</p><p>Moreover, you can define whether the files which exists in the target backup location should be preserved or deleted if they no longer exist in the source backup location. By default, such files are deleted. You can avoid it by ticking the <strong>Keep obsolete file(s)</strong> checkbox.</p><p>To begin the backup process, click on the <strong>Backup</strong> button.</p>",
    USAGE_ABORT_BACKUP_HEADER: "Abort a backup in progress",
    USAGE_ABORT_BACKUP_CONTENT: "<p>In the case you want to abort a backup which is still in progress you can do so by clicking on the <strong>Abort</strong> button. You will be prompted to confirm your choice. Click <strong>Yes</strong> if you are sure you want to abort the backup.</p><p>Please note that the backup process won't be stopped immediately! You'll have to wait until the application has finished processing the current item.</p>",
    USAGE_CHANGE_DISPLAY_LANGUAGE_HEADER: "Change application's language",
    USAGE_CHANGE_DISPLAY_LANGUAGE_CONTENT: "<p>In order to change the display language of the application, go to the <strong>Preferences</strong> dialog through the main menu or by pressing <strong>Shift+Ctrl+P</strong>, select the desired language from the list and click on the <strong>Apply</strong> button.</p>",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_HEADER: "Change maximal copy buffer size",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_CONTENT: "<p>The <strong>Preferences</strong> dialog allows you to set manually the maximal value of copy buffer size. This cannot exceed your computer's RAM size which is displayed in the Backy's <strong>Preferences</strong> dialog. As setting a wrong value can make the application to crash or harm your system, you're strongly advised to <strong>NOT</strong> to change this value unless you know exactly what you are doing!</p>",
    DOCUMENTATION_HEADER: "Technical documentation",
    DOCUMENTATION_CONTENT: "<p>The technical documentation generated by <strong>Doxygen</strong> for Backy's source code can be found <a href='technical/html/index.html'>here</a>.</p>",
    GET_BINARIES_HEADER: "Executables",
    GET_BINARIES_CONTENT: "<p>Pre-built executables of Backy will be available soon.</p>",
    GET_SOURCES_HEADER: "Build Backy from sources",
    GET_SOURCES_CONTENT: "<p>You can build Backy by yourself from its sources. Backy uses <strong>qmake</strong> to automatically generate appropriated <em>makefile</em>. Before you begin, verify that you've installed <strong>Qt®</strong> library and <strong>qmake</strong>.</p>",
    ERROR_NOT_FOUND_HEADER: "Error 404: Not found",
    ERROR_NOT_FOUND_CONTENT: "The requested page was not found on this server!",
    FOOTER: "Created and maintained by Marek Felsoci.",
    COOKIE_MESSAGE: "<p>This site uses cookies to store your display language preference as well as your choice about using cookies on this website. Do you authorize this site to use cookies?</p><p><strong>Note:</strong> The site will continue to work normally even if you does not authorize the use of cookies except that your display language preference will be lost when you reload the page or after you close the browser as well as your choice about cookies authorization.</p>",
    COOKIE_CONSENT: "Yes",
    COOKIE_DECLINE: "No"
  },
  fr: {
    NAVIGATION_ABOUT: "À propos",
    NAVIGATION_USAGE: "Guide d'utilisateur",
    NAVIGATION_DOCUMENTATION: "Documentation",
    NAVIGATION_GET: "Téléchargement",
    NAVIGATION_LANGUAGE: "Langue",

    ABOUT_INTRODUCTION_HEADER: "Qu'est-ce que Backy ?",
    ABOUT_INTRODUCTION_CONTENT: "<p>Backy est une application simple et open-source de création et de maintenance de sauvegardes. Elle est très facile à utiliser et compatible avec les systèmes d'exploitation Linux® et Windows®.</p><p>Backy permet de créer la sauvegarde soit d'un répertoire soit d'un disque entier. Il est ensuite possible de simplement mettre à jour cette sauvegarde à chaque fois que le contenu de l'emplacement source change.</p>",
    ABOUT_AUTHOR_HEADER: "Auteur",
    ABOUT_AUTHOR_CONTENT: "<p>Cette application a été développée par <a href='mailto:marek.felsoci@gmail.com'>Marek Felsoci</a>, étudiant en Cursus Master en Ingénierie spécialisé en informatique, systèmes et réseaux à l'<a href='http://www.unistra.fr' target='_blank'>Université de Strasbourg en France</a>.",
    ABOUT_CONTACT_HEADER: "Avez-vous des question, des suggestions ou souhaitez-vous contribuer à ce projet ?",
    ABOUT_CONTACT_CONTENT: "<p>Vous pouvez vous manifester auprès de l'auteur par courriel sur <a href='mailto:marek.felsoci@gmail.com'>marek.felsoci@gmail.com</a> ou grâce au <a href='https://github.com/felsocim/Backy' target='_blank'>répertoire GitHub®</a>.</p><p>Merci !</p>",
    ABOUT_LICENSE_HEADER: "Licence",
    ABOUT_LICENSE_CONTENT: "<p>Backy est publiée sous la licence <a href='https://www.gnu.org/licenses/gpl-3.0.en.html' target='_blank'>GNU General Public License version 3.0</a>.</p>",
    USAGE_INTRODUCTION_HEADER: "Comment se servir de Backy ?",
    USAGE_INTRODUCTION_CONTENT: "<p>L'interface graphique d'utilisateur de Backy est assez intuitive notamment pour les débutants.</p>",
    USAGE_CREATE_BACKUP_HEADER: "Créer une sauvegarde",
    USAGE_CREATE_BACKUP_CONTENT: "<p>Pour créer une sauvegarde vous devez spécifier l'emplacement où se trouve le contenu qui doit être sauvegardé ainsi que celui où la sauvegarde doit être créée. Ensuite, déchochez l'option <strong>Synchorniser uniquement</strong> afin de créer une nouvelle sauvegarde. Finalement, cliquez sur le bouton <strong>Lancer la sauvegarde</strong> pour démarrer le processus de sauvegarde et attendez qu'il se termine.</p>",
    USAGE_UPDATE_BACKUP_HEADER: "Mettre à jour/Synchroniser une sauvegarde",
    USAGE_UPDATE_BACKUP_CONTENT: "<p>Si auparavant vous avez déjà créé une sauvegarde de l'emplacement source séléctionné vous pouvez la synchroniser avec ce dernier sans avoir à recréer de nouveau la sauvegarde ce qui peut être coûteux en termes de temps et de l'espace disque.</p><p>Backy comparera tous les éléments dans les emplacements source et destination et copiera uniquement ceux qui ont été changés depuis la dernière sauvegarde. Vous pouvez choisir parmi deux critères de comparaison. Soit la version la plus récente soit la plus volumineuse (en termes de la taille de fichier en octets) sera conservée.</p><p>De plus, vous pouvez décider si les fichiers qui existent dans l'emplacement destinataire doivent être conservés même si ces derniers n'existent plus dans l'emplacement source. Par défaut, de tels fichiers sont suppprimés. Vous pouvez désactiver ceci en décochant l'option <strong>Conserver les fichiers obsolètes</strong>.</p><p>Pour démarrer le processus de sauvegarde cliquez sur le bouton <strong>Lancer la sauvegarde</strong>.</p>",
    USAGE_ABORT_BACKUP_HEADER: "Interrompre le processus de sauvegarde",
    USAGE_ABORT_BACKUP_CONTENT: "<p>Dans le cas où vous souhaiteriez interrompre un processus de sauvegarde qui est en cours cliquez sur le bouton <strong>Interrompre</strong>. On vous demandera de confimer votre décision. Cliquez sur <strong>Oui</strong> si vous êtes sûr de vouloir interrompre le processus de sauvegarde.</p><p>Veuillez noter que ce dernier ne sera pas interrompu immédiatement ! Le logiciel devra d'abord terminer le traitement de l'élément courant.</p>",
    USAGE_CHANGE_DISPLAY_LANGUAGE_HEADER: "Changer la langue de l'interface d'utilisateur",
    USAGE_CHANGE_DISPLAY_LANGUAGE_CONTENT: "<p>Afin de changer la langue de l'interface d'utilisateur activer la fenêtre <strong>Préférences</strong> en passant par le menu principal ou grâce au raccourci clavier <strong>Shift+Ctrl+P</strong>, séléctionnez la langue souhaitée et cliquez sur le bouton <strong>Enregistrer</strong>. Le changement sera effectif après le redémarrage de l'application.</p>",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_HEADER: "Changer la taille maximale du tampon de copie de fichier(s)",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_CONTENT: "<p>La fenêtre de <strong>Préférences</strong> vous permet de saisir manuellement la valeur maximale de la taille du tampon de copie de fichier(s). Celle-ci ne peut pas dépasser la tailler de la mémoire vive disponible sur votre ordinateur qui est affichée également dans la fenêtre de <strong>Préférences</strong>. Puisque la saisie d'une valeur incorrecte peut causer la terminaison impromptue de l'application ou endommager votre système, nous vous récommandons de ne pas changer cette valeur à moins que vous sachiez exactement ce que vous faites !</p>",
    DOCUMENTATION_HEADER: "Documentation technique",
    DOCUMENTATION_CONTENT: "<p>La documentation technique complète du code source de Backy générée par <strong>Doxygen</strong> est disponible en suivant <a href='technical/html/index.html'>ce lien</a>.</p>",
    GET_BINARIES_HEADER: "Exécutables",
    GET_BINARIES_CONTENT: "<p>Les exécutables pré-compilés de Backy vont être bientôt mis à disposition ici.</p>",
    GET_SOURCES_HEADER: "Compiler Backy à partir de ses fichiers sources",
    GET_SOURCES_CONTENT: "<p>Vous pouvez compiler Backy par vous-mêmes à partir de ses fichiers sources. Backy utilise <strong>qmake</strong> pour générer automatiquement le fichier <em>makefile</em>. Avant de commencer, vérifier si vous avez installé la librarie <strong>Qt®</strong> ainsi que <strong>qmake</strong>.</p>",
    ERROR_NOT_FOUND_HEADER: "Erreur 404: Page introuvable",
    ERROR_NOT_FOUND_CONTENT: "La page que vous cherchez n'existe pas sur ce site !",
    FOOTER: "Créé et maintenu par Marek Felsoci.",
    COOKIE_MESSAGE: "<p>Ce site utilise des cookies pour se rappeler de votre choix de langue et de votre décision sur l'utilisation de cookies sur ce site. Autorisez-vous ce site à utiliser des cookies ?</p><p><strong>Information :</strong> Le site va continuer à fonctionner normalement mais il ne pourra pas se rappeler de votre choix de langue si vous recharger la page ou après la fermeture du navigateur ni de votre décision concernant l'utilisation de cookies.</p>",
    COOKIE_CONSENT: "Oui",
    COOKIE_DECLINE: "Non"
  },
  sk: {
    NAVIGATION_ABOUT: "O programe",
    NAVIGATION_USAGE: "Príručka",
    NAVIGATION_DOCUMENTATION: "Dokumentácia",
    NAVIGATION_GET: "Stiahnutie",
    NAVIGATION_LANGUAGE: "Jazyk",

    ABOUT_INTRODUCTION_HEADER: "Čo je Backy?",
    ABOUT_INTRODUCTION_CONTENT: "<p>Backy je jednoduché riešenie na vytváranie a správu záloh s otvoreným zdrojovým kódom. Veľmi jednoducho sa používa je kompatibilné s operačnými systémami Linux® aj Windows®.</p><p>Backy umožňuje vytvárať zálohy priečinkov alebo celých diskových jednotiek. Potom je možné vytvorenú zálohu aktualizovať zakaždým keď sa obsah zdrojového umiestnenia zmení.</p>",
    ABOUT_AUTHOR_HEADER: "Tvorca",
    ABOUT_AUTHOR_CONTENT: "<p>Tento program napísal <a href='mailto:marek.felsoci@gmail.com'>Marek Felšöci</a>, IT študent <a href='http://www.unistra.fr' target='_blank'>Štrasburskej univerzity vo Francúzsku</a>.",
    ABOUT_CONTACT_HEADER: "Máte nejaké otázky, pripomienky alebo chuť sa podieľať na tomto projekte?",
    ABOUT_CONTACT_CONTENT: "<p>Prosím, podeľte sa s nimi s tvorcom prostredníctvom e-mailu na adrese <a href='mailto:marek.felsoci@gmail.com'>marek.felsoci@gmail.com</a> alebo cez <a href='https://github.com/felsocim/Backy' target='_blank'>GitHub® depozitár</a> projektu.</p><p>Ďakujeme!</p>",
    ABOUT_LICENSE_HEADER: "Licencia",
    ABOUT_LICENSE_CONTENT: "<p>Backy je šírený pod licenciou <a href='https://www.gnu.org/licenses/gpl-3.0.en.html' target='_blank'>GNU General Public License version 3.0</a>.</p>",
    USAGE_INTRODUCTION_HEADER: "Ako používať Backyho?",
    USAGE_INTRODUCTION_CONTENT: "<p>Grafické používateľské rozhranie Backyho je značne intuitívne najmä pre začiatočníkov.</p>",
    USAGE_CREATE_BACKUP_HEADER: "Vytvoriť zálohu",
    USAGE_CREATE_BACKUP_CONTENT: "<p>Pre vytvorenie zálohy musíte zadať cestu k umiestneniu obsahu, ktorý chcete zálohovať a zároveň aj cestu k umiestneniu, kde má byť záloha vytvorená. Potom odznačte možnosť <strong>Iba aktualizovať</strong> pre vytvorenie úplne novej zálohy. Nakoniec kliknite na tlačidlo <strong>Zálohovať</strong> pre spustenie procesu zálohovania a čakajte kým sa nedokončí.</p>",
    USAGE_UPDATE_BACKUP_HEADER: "Aktualizovať/Synchronizovať zálohu",
    USAGE_UPDATE_BACKUP_CONTENT: "<p>Ak už máte vytvorenú zálohu uvedeného zdrojového umiestnenia, môžete ju s ním jednoducho synchronizovať bez toho, aby ste museli znova vytvárať ďalšiu zálohu čo môže byť nákladné čo sa týka času a miesta na disku.</p><p>Backy porovná všetky položky v zdrojovom a cieľovom umiestnení a skopíruje len tie, ktoré boli zmenené od posledného zálohovania. Môžete si vybrať spomedzi dvoch porovnávacích kritérií. Buď sa zachovajú tie najnovšie alebo tie najväčšie (tzn. veľkosť súboru v bajtoch) verzie súborov.</p><p>Môžete si vybrať aj to, či sa súbory, ktoré stále jestvujú v cieľovom umiestnení ale už sa nenachádzajú v zdrojovom umiestnení zálohy, odstránia alebo budú zachované. Predvolene sa takéto súbory odstraňujú. Toto nastavenie môžete zmeniť tým, že označíte možnosť <strong>Zachovať nepotrebné súbory</strong>.</p><p>Pre spustenie zálohovania kliknite na tlačidlo <strong>Zálohovať</strong>.</p>",
    USAGE_ABORT_BACKUP_HEADER: "Prerušiť prebiehajúce zálohovanie",
    USAGE_ABORT_BACKUP_CONTENT: "<p>V prípade, že chcete prerušiť prebiehajúci proces zálohovania kliknite na tlačidlo <strong>Prerušiť</strong>. Buete požiadaný o potvrdenie vašej voľby. Vyberte možnosť <strong>Áno</strong> ak ste si istý, že chcete prerušiť zálohovanie.</p><p>Vedzte, že proces zálohovania nebude prerušený okamžite! Najprv bude dokončené spracovanie aktuálnej položky.</p>",
    USAGE_CHANGE_DISPLAY_LANGUAGE_HEADER: "Zmeniť jazyk programu",
    USAGE_CHANGE_DISPLAY_LANGUAGE_CONTENT: "<p>Pre zmenu jazyka prejdite do okna <strong>Predvoľby</strong> cez hlavnú ponuku programu alebo pomocou klávesovej skratky <strong>Shift+Ctrl+P</strong>, vyberte požadovaný jazyk a kliknite na tlačidlo <strong>Uložiť</strong>.</p>",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_HEADER: "Zmeniť maximálnu veľkosť zásobníka kopírovania",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_CONTENT: "<p>Okno <strong>Predvoľby</strong> umožňuje ručne nastaviť najväčšiu možnú veľkosť zásobníka kopírovania. Táto hodnota nemôže prekročiť veľkosť pamäte RAM vášho počítača, ktorá je tiež zobrazená v okne <strong>Predvoľby</strong>. Pretoež nastavenie zlej hodnoty môže spôsobiť pád programu alebo dokonca aj celého systému, neodporúčame vám túto hodnotu meniť ak si nie ste úplne istý tým, čo robíte!</p>",
    DOCUMENTATION_HEADER: "Technická dokumentácia",
    DOCUMENTATION_CONTENT: "<p>Technická dokumentácia zdrojového kódu programu vygenerovaná pomocou nástroja <strong>Doxygen</strong> je dostupná <a href='technical/html/index.html'>na tejto stránke</a>.</p>",
    GET_BINARIES_HEADER: "Spustiteľné súbory",
    GET_BINARIES_CONTENT: "<p>Skompilované spustiteľné súbory programu Backy zatiaľ nie sú dostupné.</p>",
    GET_SOURCES_HEADER: "Zostaviť Backyho zo zdrojových súborov",
    GET_SOURCES_CONTENT: "<p>Môžete si skompilovať Backyho aj sami z jeho zdrojových kódov. Backy používa <strong>qmake</strong> na generovanie súboru <em>makefile</em>. Predtým, než začnete sa uistite, že máte nainštalovanú knižnicu <strong>Qt®</strong> a nástroj  <strong>qmake</strong>.</p>",
    ERROR_NOT_FOUND_HEADER: "Chyba 404: Stránka sa nenašla",
    ERROR_NOT_FOUND_CONTENT: "Stránka, ktorú hľadáte sa na tomto serveri nenachádza!",
    FOOTER: "Vytvoril a spravuje Marek Felsoci.",
    COOKIE_MESSAGE: "<p>Tieto stránky používajú súbory cookie na zapamätanie si vašej voľby jazyka a vášho súhlasu na používanie súborov cookie na týchto stránkach. Súhlasíte aby tieto stránky používali súbory cookie?</p><p><strong>Poznámka:</strong>Funkcie stránok nebudú obmedzené aj keď zablokujete používanie súborov cookie, lenže váš výber jazyka a vaše rozhodnutie o používaní súborov cookie budú stratené po obnovení stránky či ukončení vášho prehliadača.</p>",
    COOKIE_CONSENT: "Áno",
    COOKIE_DECLINE: "Nie"
  }
};

function translate(identifier, language = defaultLanguage) {
  if(language in strings) {
    return identifier in strings[language] ? strings[language][identifier] : strings[defaultLanguage][identifier];
  }

  return "N/A";
}

export { defaultLanguage, translate };
