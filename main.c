#include <limits.h>
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void reverse(char *str);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("You need to add a Path to the .AppImage");
    return 0;
  }

  char PATH[PATH_MAX];

  if (realpath(argv[1], PATH) != NULL) {
    printf("Full PATH = %s", PATH);
  }

  const char *HOME = getenv("HOME");

  const char apps[] = "/.local/share/applications";

  size_t lenHOME = strlen(HOME);
  size_t lenApps = strlen(apps);

  char *ApplicationsPath = malloc(lenHOME + lenApps + 1);

  if (!ApplicationsPath) {
    return 1;
  }

  strcpy(ApplicationsPath, HOME);
  strcat(ApplicationsPath, apps);

  printf("Applications Path: %s\n", ApplicationsPath);

  char appName[128] = "";

  for (int i = strlen(PATH); i > 0; i--) {
    char c = PATH[i];
    printf("Looping %c\n", c);
    if (PATH[i] == '/') {
      printf("Breaking CHAR: %c", c);
      break;
    }
    size_t nameLen = strlen(appName);
    appName[nameLen] = c;
    appName[nameLen + 1] = '\0';
  }

  reverse(appName);

  char *dot = strchr(appName, '.');

  if (dot != NULL) {
    *dot = '\0';
  }

  size_t lenAppsPath = strlen(ApplicationsPath);
  size_t lenAppName = strlen(appName);

  char *DesktopPath = malloc(lenAppsPath + lenAppName + 10);

  strcpy(DesktopPath, ApplicationsPath);
  strcat(DesktopPath, "/");
  strcat(DesktopPath, appName);
  strcat(DesktopPath, ".desktop");
  printf("appName: %s", appName);
  printf("\nPlease enter the Description: ");
  char desc[256];

  fgets(desc, sizeof(desc), stdin);

  FILE *pFILE = fopen(DesktopPath, "w");
  if (pFILE == NULL) {
    perror("Error while opening file");
    return 1;
  }
  fprintf(pFILE, "[Desktop Entry]");
  fprintf(pFILE, "\nType=Application");
  fprintf(pFILE, "\nName=%s", appName);
  fprintf(pFILE, "\nComment=%s", desc);
  fprintf(pFILE, "\nExec=%s", PATH);
  fprintf(pFILE, "\nTerminal=false");

  fclose(pFILE);

  return 0;
}

void reverse(char *str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = temp;
  }
}
