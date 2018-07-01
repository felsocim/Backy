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
    USAGE_INTRODUCTION_CONTENT: "<p>Backy's graphical user interface is very intuitive notable for beginner users. See some illustrative examples in the screenshot section.</p>",
    USAGE_CREATE_BACKUP_HEADER: "Create a backup",
    USAGE_CREATE_BACKUP_CONTENT: "<p>To create a backup, you need to specify the location where the content to be backup up is stored as well as the location where the backup will be created. Then uncheck the <strong>Synchronize only</strong> checkbox in order to create a fresh backup. Finally, click on the <strong>Backup</strong> button to begin the backup process and wait until it completes.</p>",
    USAGE_UPDATE_BACKUP_HEADER: "Update/Synchronize a backup",
    USAGE_UPDATE_BACKUP_CONTENT: "<p>If you already had done a backup of the selected source location, you can synchronize it with the most recent changes of the latter without having to re-create an another backup which can become very disk space-consuming.</p><p>Backy will compare all the items in the source and the target backup locations and copy only those that had been changed since the last backup. You can choose between two comparison criterions. Either the most recent version or the biggest (in terms of file size) version of an item will be kept.</p><p>Moreover, you can define whether the files which exists in the target backup location should be preserved or deleted if they no longer exist in the source backup location. By default, such files are deleted. You can avoid it by ticking the <strong>Keep obsolete file(s)</strong> checkbox.</p><p>To begin the backup process, click on the <strong>Backup</strong> button.</p>",
    USAGE_ABORT_BACKUP_HEADER: "Abort a backup in progress",
    USAGE_ABORT_BACKUP_CONTENT: "<p>In the case you want to abort a backup which is still in progress you can do so by clicking on the <strong>Abort</strong> button. You will be prompted to confirm your choice. Click <strong>Yes</strong> if you are sure you want to abort the backup.</p><p>Please note that the backup process won't be stopped immediately! You'll have to wait until the application has finished processing the current item.</p>",
    USAGE_CHANGE_DISPLAY_LANGUAGE_HEADER: "Change application's language",
    USAGE_CHANGE_DISPLAY_LANGUAGE_CONTENT: "<p>In order to change the display language of the application, go to the <strong>Preferences</strong> dialog through the main menu or by pressing <strong>Shift+Ctrl+P</strong>, select the desired language from the list and click on the <strong>Apply</strong> button.</p>",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_HEADER: "Change maximal copy buffer size",
    USAGE_MAXIMAL_COPY_BUFFER_SIZE_CONTENT: "<p>The <strong>Preferences</strong> dialog allows you to set manually the maximal value of copy buffer size. This cannot exceed your computer's RAM size which is displayed in the Backy's <strong>Preferences</strong> dialog. As setting a wrong value can make the application to crash, you're strongly advised to <strong>NOT</strong> to change this value unless you know exactly what you are doing!</p>",
    DOCUMENTATION_HEADER: "Technical documentation",
    DOCUMENTATION_CONTENT: "<p>The technical documentation generated by <strong>Doxygen</strong> for Backy's source code can be found <a href='technical/html/index.html'>here</a>.</p>",
    GET_BINARIES_HEADER: "Executables",
    GET_BINARIES_CONTENT: "<p>Pre-built executables of Backy will be available soon.</p>",
    GET_SOURCES_HEADER: "Build Backy from sources",
    GET_SOURCES_CONTENT: "<p>You can build Backy by yourself from its sources. Backy uses <strong>qmake</strong> to automatically generate appropriated <em>makefile</em>. Before you begin, verify that you've installed <strong>Qt®</strong> library and <strong>qmake</strong>.</p>",
    ERROR_NOT_FOUND_HEADER: "Error 404: Not found",
    ERROR_NOT_FOUND_CONTENT: "The requested page was not found on this server!",
    FOOTER: "Copyleft 2018 by Marek Felsoci."
  }
};

function translate(identifier, language = defaultLanguage) {
  if(language in strings) {
    return identifier in strings[language] ? strings[language][identifier] : strings[defaultLanguage][identifier];
  }

  return "N/A";
}

export { defaultLanguage, translate };