XML Editor   (c) by Martin Gäckler Linz - https://www.gaeckler.at/ - 22.01.2014
===============================================================================

XML Edit ist ein Editor für XML-dateien. Die Benutzung ist kostenlos, 
allerdings übernemme ich keinerlei Gewähr für die Nutung der 
Software.

Das Editorfenster ist in drei Teile gegliedert.

- Links ist die Dokumentstruktur der XML- bzw. HTML-Datei
- Rechts oben werden die Attribute des ausgewählten XML-Elements angezeigt und
  können auch geändert werden.
- Rechts unten sehen Sie den Wert des ausgeählten XML-Elements. Bei manchen
  XML-Elementen wie z.B. PCDATA können Sie diesen auch ändern.
  
Wenn die XML-Datei mit einem XSL-Stylesheet verknüpft wurde, können Sie im Menü
Transform dieses Anwenden.

Wenn die Datei mit einem Schema verknüpft ist, können Sie die Gültigkeit der 
XML-Datei prüfen lassen. Außerdem werden Ihnen beim Erzeugen neuer Elemente
die entsprechenden XML-Tags angeboten.

Mit der XML-Datei wird, falls vorhanden, die Datei editor.css geladen. Diese 
Datei wird sowohl im Vereichnis der XML-Datei als auch im Programmvereichniss 
gesucht und geladen. Wenn mit der Datei ein CSS-Stylesheet verknüpft wurde, so 
wird auch diese Datei geladen.

Im Menüedit, befindet sich ein Punkt Renderer, welcher die XML-Datei unter 
Berücksichtigung aller geladenen CSS-Regeln anzeigt.

Im Popupmenü der Dokumentstruktur, können Sie sich die angewendeten CSS-Styles 
des ausgewählen Elements anzeigen lassen.

Bitte beachten Sie:
Nicht alle Elemente eines Schemas oder der XSLT-Transformation wurden 
implementiert. In den Dateien xschema.xsd und xslt.xsd finden Sie die 
Beschreibung der unterstützten Elemente.

Ebenso wurden noch nicht alle CSS-Styles implementiert. Welche interpretiert 
werden, können Sie dem Formular für die Anzeige der Styles entnehmen. 
Stilangaben, die nicht angezeigt werden, wurden auch nicht implementiert. Für 
background-image gilt jedoch: die Angabe wird zwar gelesen, jedoch wird das 
Hintergrundbild weder geladen noch angezeigt.

