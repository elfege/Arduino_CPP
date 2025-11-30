 client.print("<form method=get><br><font size='6'>Red:  </font></h1><input type='range' min='1' max='100' name=redVal value=");
  client.print(redTemp);  //set the value of the slider based upon the previous page load value
  client.println(" style='height:30px; width:200px' oninput='showRedValue(this.value)'>");    //was onchange event
  client.print("<span id='redrange'>");
  client.print(redTemp);  //set the javascript initial value
  client.println("</span>");
  client.println("<script type='text/javascript'>\r\nfunction showRedValue(newValue)\r\n{\r\ndocument.getElementById('redrange').innerHTML=newValue;\r\n}\r\n</script>\r\n");
