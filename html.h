
const char index_html[]  = R"rawliteral(
<!DOCTYPE html>
  <html>
    <head>
      <title>BexxiGun</title>
    </head>
    <body>
      <h1>BexxiGun Web-Control</h1>
      <p><strong><div id="motorState"></div></strong></p>
      <p>last dart speed: <div id="dartSpeed"></div> </p>
      <p>max dart speed: <div id="maxdartSpeed"></div> </p>
      <p>min dart speed: <div id="mindartSpeed"></div> </p>
      <div id="gunstateSelect">
        <button onclick="SendPARTY();" id="partybutton" value="PARTY_MODE">PARTY MODE</button>
        <button onclick="SendDTH();" id="deathbutton" value="DEATH_MODE">DEATH MODE</button>
        <button onclick="SendSNEAK();" id="sneakbutton" value="SNEAK_MODE">SNEAK MODE</button>
        <button onclick="SendTVBGONE();" id="tvbgonebutton" value="TV_B_GONE_MODE">TV B GONE MODE</button>
      </div>
      <div><button onclick="SendColor();" id="hcolorbutton" >COLOR1</button>  <input type="range" id="hcolor" min="0" max="255" onchange="changeBG()" /></div>
    </body>
 <script>

  function changeBG(){
      let col=(((document.getElementById("hcolor").value)*360)>>8);
      console.log("hsl("+col+"deg,100%%,50%%)");
      let colhsv ="hsl("+col+"deg,100%%,50%%)";
      //document.body.style.backgroundColor.style.backgroundColor = "hsl("+col+"deg,100%%,50%%)";
      document.getElementById("hcolorbutton").style.backgroundColor = colhsv;
  }

  function setUpGunstateSelect(gunState){
    buttons = document.getElementById("gunstateSelect").children
    for (let i = 0; i < buttons.length; i++) {
      if(i == gunState){
        buttons[i].style.backgroundColor ="blue";
      }
      else{
        buttons[i].style.backgroundColor ="white";
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
    console.log("SEND Sneak");
  }

  function SendTVBGONE() {
    console.log("SEND TV b Gone");
    let xhr = new XMLHttpRequest();
    xhr.open("POST", window.location.href+document.getElementById("tvbgonebutton").value);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log(xhr.status);
      console.log(xhr.responseText);
    }};

    xhr.send();
    console.log("SEND TV b Gone");
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

  setInterval(function GetGunState(){
    console.log("GET GUN STATE");
    let xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
      if (xhr.readyState === 4 && xhr.status === 200) {
        var json = JSON.parse(xhr.responseText);
        setUpGunstateSelect(json.gunstate);
        document.getElementById("dartSpeed").innerHTML = json.dartspeed;
        document.getElementById("maxdartSpeed").innerHTML = json.maxdartspeed;
        document.getElementById("mindartSpeed").innerHTML = json.mindartspeed;
        document.getElementById("motorState").innerHTML = json.motorstate;
      }
    };
    xhr.open("GET", window.location.href+"GUNDATA");
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xhr.send();
    console.log("SEND GET GUN STATE");

  },3000);

 setUpGunstateSelect();
  </script>

  </html>

  
)rawliteral";

