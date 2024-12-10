XML Editor   (c) by Martin G�ckler Linz - https://www.gaeckler.at/ - 22.01.2014
===============================================================================

XML Edit ist ein Editor f�r XML-dateien. Die Benutzung ist kostenlos, 
allerdings �bernemme ich keinerlei Gew�hr f�r die Nutung der 
Software.

Das Editorfenster ist in drei Teile gegliedert.

- Links ist die Dokumentstruktur der XML- bzw. HTML-Datei
- Rechts oben werden die Attribute des ausgew�hlten XML-Elements angezeigt und
  k�nnen auch ge�ndert werden.
- Rechts unten sehen Sie den Wert des ausge�hlten XML-Elements. Bei manchen
  XML-Elementen wie z.B. PCDATA k�nnen Sie diesen auch �ndern.
  
Wenn die XML-Datei mit einem XSL-Stylesheet verkn�pft wurde, k�nnen Sie im Men�
Transform dieses Anwenden.

Wenn die Datei mit einem Schema verkn�pft ist, k�nnen Sie die G�ltigkeit der 
XML-Datei pr�fen lassen. Au�erdem werden Ihnen beim Erzeugen neuer Elemente
die entsprechenden XML-Tags angeboten.

Mit der XML-Datei wird, falls vorhanden, die Datei editor.css geladen. Diese 
Datei wird sowohl im Vereichnis der XML-Datei als auch im Programmvereichniss 
gesucht und geladen. Wenn mit der Datei ein CSS-Stylesheet verkn�pft wurde, so 
wird auch diese Datei geladen.

Im Men�edit, befindet sich ein Punkt Renderer, welcher die XML-Datei unter 
Ber�cksichtigung aller geladenen CSS-Regeln anzeigt.

Im Popupmen� der Dokumentstruktur, k�nnen Sie sich die angewendeten CSS-Styles 
des ausgew�hlen Elements anzeigen lassen.

Bitte beachten Sie:
Nicht alle Elemente eines Schemas oder der XSLT-Transformation wurden 
implementiert. In den Dateien xschema.xsd und xslt.xsd finden Sie die 
Beschreibung der unterst�tzten Elemente.

Ebenso wurden noch nicht alle CSS-Styles implementiert. Welche interpretiert 
werden, k�nnen Sie dem Formular f�r die Anzeige der Styles entnehmen. 
Stilangaben, die nicht angezeigt werden, wurden auch nicht implementiert. F�r 
background-image gilt jedoch: die Angabe wird zwar gelesen, jedoch wird das 
Hintergrundbild weder geladen noch angezeigt.

