--- src/DirectoryIndexing.cxx.orig	2005-10-04 07:45:52 UTC
+++ src/DirectoryIndexing.cxx
@@ -518,6 +518,7 @@ char GenerateFolderIndex( string ArgVirt
 
 		// <html> and <head>
 		ArgBuffer = "<html>\n<head>\n<title>Directory listing for " + string(ArgVirtualPath) + "</title>\n";
+		ArgBuffer += "<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\" />\n";
 		// A clone of KDE's public fileserver CSS sheet
 		ArgBuffer += "<style type=\"text/css\">\n<!--\n";
 		ArgBuffer += "body { font-family: Verdana, Arial, sans-serif;}\n#DFS_table {color: rgb(0, 0, 0);background-color: rgb(234, 233, 232); border: thin outset; width: 100%; }\n";
@@ -528,7 +529,9 @@ char GenerateFolderIndex( string ArgVirt
 		ArgBuffer += ".DFS_entrytype { text-align: center; width: 6em; }\ntd.DFS_entrytype { background-color: rgb(255, 255, 255); font-weight: bold; color: rgb(150, 150, 150);}\n";
 		ArgBuffer += ".DFS_entrysize { color: rgb(0, 0, 0); text-align: right;}\na.DFS_direntry { font-weight: bold; }\n";
 		ArgBuffer += "-->\n</style>\n";
+		ArgBuffer += "</head>\n";
 		// Insert the table.
+		ArgBuffer += "<body>\n";
 		ArgBuffer += InsertIndexTable( DirectoryPointer, VirtualPath, string( ArgPath ), Filler );
 		// Server Version Information.
 		ArgBuffer += "<hr><i>DashFileServer [Version " + string(MAJORVERSION) + "." + string(MINORVERSION) + "." + string(PATCHVERSION) + "]</i>\n";
