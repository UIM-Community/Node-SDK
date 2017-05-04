#include <nimbus.h>
#include <nim.h>
#include <port.h>
#include <pds.h>

/* Global variables */
char *gProbeName;
char *gVersion;

/********************************************************
 * list_robots - example on how to perform calls to other
 *               Nimsoft components
 ********************************************************/
int list_robots () {
    PDS *psIn=NULL, *psOut=NULL, *psEntry=NULL;
    int i=0, iRes=0, iWait=30;
    char *Tmp=NULL, *HubDomain=NULL, *HubName=NULL, *HubRobotName=NULL;
    const char *Err=NULL;
    char zHubAddress [512]="";

    /* Issue the gethub command to the local controller */
    iRes = nimNamedRequest ("controller", "gethub", psIn, &psOut, iWait);
    if (iRes != NIME_OK) {
        Err = nimError2Txt (iRes);
        return FALSE;
    }

    /* Get the hubdomain, hubname and hubrobot entries from the returned PDS */
    if (pdsGet_PCH (psOut, "hubdomain", &HubDomain) != PDS_ERR_NONE) {
        pdsDelete (psOut);
        return FALSE;
    }
    if (pdsGet_PCH (psOut, "hubname", &HubName) != PDS_ERR_NONE) {
        free (HubDomain);
        pdsDelete (psOut);
        return FALSE;
    }
    if (pdsGet_PCH (psOut, "hubrobotname", &HubRobotName) != PDS_ERR_NONE) {
        free (HubDomain);
        free (HubName);
        pdsDelete (psOut);
        return FALSE;
    }
    pdsDelete (psOut);
    psOut = NULL;

    /* Build a full Nimsoft address for the Hub */
    sprintf (zHubAddress, "/%s/%s/%s/hub", HubDomain, HubName, HubRobotName);
    free (HubDomain);
    free (HubName);
    free (HubRobotName);

    /* Issue the getrobots command to the hub */
    iRes = nimNamedRequest (zHubAddress, "getrobots", psIn, &psOut, iWait);
    if (iRes != NIME_OK) {
        Err = nimError2Txt (iRes);
        return FALSE;
    }

    /* Loop through the robotlist in the return PDS one element at a time */
    psEntry = NULL;
    i = 0;
    while (pdsGetTable (psOut, PDS_PDS, "robotlist", i, &psEntry) == PDS_ERR_NONE) {
        if (pdsGet_PCH (psEntry, "name", &Tmp) == PDS_ERR_NONE) {
            free (Tmp);
        }
        pdsDelete (psEntry);
        i++;
    }

    pdsDelete (psOut);
    return TRUE;
}


/***************
 * main program
 ***************/
int main (int argc, char **argv) {
    extern char *optarg;
    extern int optind;
    int opt=0;

    gProbeName = strdup ("example_probe");
    gVersion = strdup ("1.00");

    nimInit (0);

    /* Parse command-line options */
    while ((opt = getopt (argc, argv, "Vh")) != EOF) {
        switch (opt) {
        case 'V':                   /* Get version information */
            printf ("%s %s\n", gProbeName, gVersion);
            exit (0);
            break;
        case 'h':
        default:
            printf ("usage: %s [Vh]\n", gProbeName);
            exit (1);
            break;
        }
    }


    /* Perform nimsoft calls */
    list_robots ();

    nimEnd (0);

    if (gProbeName) free (gProbeName);
    if (gVersion) free (gVersion);
    exit (0);
}
