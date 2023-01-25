void serialmonitoring() {

  // gestion ecriture dans variable
  while (Serial.available()) {  // parametrage pour envoi de commandes par le monitor serie
    smoothgainmap = Serial.readString().toDouble();
  }

  //


  Serial.print("smoothgainmap=");
  Serial.print(smoothgainmap);
  Serial.print("          newvalue=");
  Serial.print(newvalue);

  Serial.print("       smooth=");
  Serial.print(smooth);

  Serial.print("       difference=");
  Serial.println(difference);
}