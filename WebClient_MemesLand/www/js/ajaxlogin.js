function deleteAllCookies() {
    var cookies = document.cookie.split(";");

    for (var i = 0; i < cookies.length; i++) {
        var cookie = cookies[i];
        var eqPos = cookie.indexOf("=");
        var name = eqPos > -1 ? cookie.substr(0, eqPos) : cookie;
        document.cookie = name + "=;expires=Thu, 01 Jan 1970 00:00:00 GMT";
    }
}

function getCookie(cname) {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') c = c.substring(1);
        if (c.indexOf(name) != -1) {
			return c.substring(name.length, c.length);
		}
    }
    return "";
}

function logout() {
	deleteAllCookies();
	setForm();
	document.location.reload(true);
	return false;
}

function processForm() {
	var msg = getCookie('usermsg');
	var name = getCookie('username');
	
	if (msg == '' || name == '') {
		setForm();
		return false;
	}
	else {
		setMessage(msg);
		return true;
	}
}

function setMessage(msg) {
	document.getElementById('loginform').innerHTML = '<p class="adv">' + msg + '</p>' + '<input class="log button" type="submit" name="submit" value="Se déconnecter" onclick="return logout();">';
}

function setError(msg) {
	document.getElementById('loginform').innerHTML = '<p class="adv">' + msg + '</p>' + '<input class="log button" type="submit" name="submit" value="Retour" onclick="return logout();">';
}

function setForm() {
	document.getElementById('loginform').innerHTML = '<form id="formlog">  \
			<p class="adv">Identifiant</p> <input class="log" type="text" name="username" id="unlog" value="" />\
			<br />\
			<p class="adv">Mot de passe</p> <input class="log" type="password" name="userpwd" id="uplog" value="" />\
			<br />\
			<br />\
			<input class="log button" type="submit" name="submit" value="Se connecter" id="uslog">\
			<br />\
		</form>\
		<a class="centerf" href="register.html"><p>Creer un compte</p></a>\
		';
}

window.onload = function() {
	var logged = processForm();
	if (document.getElementById("formreg") != null) {
		registerOnload();
	}
	if (logged == true) {
		return true;
	}
	document.getElementById("formlog").onsubmit = function(event) {
		event.preventDefault();
		if (window.ActiveXObject) {
			xhr = new ActiveXObject("Microsoft.XMLHTTP");
		}
		else {
			xhr = new XMLHttpRequest();
		}
	
		xhr.onreadystatechange = function(){
			if (this.readyState == 4) {
				if (this.status == 200) {
					var test = xhr.responseText.split(' ');
					if (test[0] == "Bonjour") {
						document.cookie = "username=" + username + "; path=/";
						document.cookie = "usermsg=" + xhr.responseText + "; path=/";
						setMessage(xhr.responseText);
						document.location.reload(true);
					}
					else {
						setError(xhr.responseText);
					}
				}
				else {
					alert("Error");
				}
			}
		};
	
		var username = document.getElementById('unlog').value;
		var userpwd = document.getElementById('uplog').value;

		xhr.open("POST", "/htbin/login.py", true);
		xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
		xhr.send("username=" + username + "&userpwd=" + userpwd);

		return true;
	}
}