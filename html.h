
const char index_html[]  = R"rawliteral(
<!DOCTYPE html>
  <html>
    <head>
      <title>BexxiGun</title>
      <meta http-equiv="refresh" content="10" />
      <script>
        function changeButtonColor(){
          let col=(((document.getElementById("hcolor").value)*360)>>8);
          console.log("hsl("+col+"deg,100%%,50%%)");
          document.getElementById("hcolorbutton").style.backgroundColor = "hsl("+col+"deg,100%%,50%%)";
        }
      </script>
    </head>
    <body>
      <h1>BexxiGun 0.1 Webinterface 1</h1>
      <p>MOTOR STATE: <strong> %MOTORSTATE%</strong></p>
      <div id="gunstateSelect">
        <button onclick="SendPARTY();" id="partybutton" value="PARTY_MODE">PARTY MODE</button>
        <button onclick="SendDTH();" id="deathbutton" value="DEATH_MODE">DEATH MODE</button>
        <button onclick="SendSNEAK();" id="sneakbutton" value="SNEAK_MODE">SNEAK MODE</button>
      </div>
      <div><button onclick="SendColor();" id="hcolorbutton" >COLOR1</button>  <input type="range" id="hcolor" min="0" max="255" onchange="changeButtonColor()" /></div>
    </body>
 <script>
   gunState = %GUNSTATE%
 

  function setUpGunstateSelect(){
    buttons = document.getElementById("gunstateSelect").children
    for (let i = 0; i < buttons.length; i++) {
      if(i == gunState){
        buttons[i].style.backgroundColor ="blue";
      }
    } 
  } 

  
  function SendPARTY() {
    console.log("SEND Party");
    let xhr = new XMLHttpRequest();
    xhr.open("POST", window.location.href+document.getElementById("partybutton").value);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log(xhr.status);
      console.log(xhr.responseText);
    }};

    xhr.send();
    console.log("SEND Party");
  }

  function SendDTH() {
    console.log("SEND Death");
    let xhr = new XMLHttpRequest();
    xhr.open("POST", window.location.href+document.getElementById("deathbutton").value);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log(xhr.status);
      console.log(xhr.responseText);
    }};

    xhr.send();
    console.log("SEND Death");
  }

  function SendSNEAK() {
    console.log("SEND Sneak");
    let xhr = new XMLHttpRequest();
    xhr.open("POST", window.location.href+document.getElementById("sneakbutton").value);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log(xhr.status);
      console.log(xhr.responseText);
    }};

    xhr.send();
    console.log("SEND Death");
  }

  function SendColor() {
    console.log("SEND COLOR");
    let xhr = new XMLHttpRequest();
    xhr.open("POST", window.location.href+"COLOR?color="+document.getElementById("hcolor").value);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log(xhr.status);
      console.log(xhr.responseText);
    }};


    xhr.send();
    console.log("SEND COLOR222");
  }

 setUpGunstateSelect();
  </script>

  </html>

  
)rawliteral";

