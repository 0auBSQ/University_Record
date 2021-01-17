function registerOnload() {
	document.getElementById("formreg").onsubmit = function() {
		var lastname = document.register.lastname.value;
		var firstname = document.register.firstname.value;
		var birthdate = document.register.birthdate.value;
		var username = document.register.username.value;
		var userpwd = document.register.userpwd.value;
		var usercpwd = document.register.usercpwd.value;
		var useremail = document.register.useremail.value;

		checkFirstname(firstname);
		checkCPassword(usercpwd);
		checkLastname(lastname);
		checkEmail(useremail);
		checkPassword(userpwd);
		checkUsername(username);
		checkDate(birthdate);
		if (checkFirstname(firstname) && checkCPassword(usercpwd) && checkLastname(lastname) && checkEmail(useremail) && checkPassword(userpwd) && checkUsername(username) && checkDate(birthdate)) {
			document.getElementById("successmsg").textContent = "Inscription reussie !";
			return (true);
		}
		return (false);
	}
	document.getElementById("datereg").oninput = function() {
		checkDate(this.value);
	}
	document.getElementById("firstreg").oninput = function() {
		checkFirstname(this.value);
	}
	document.getElementById("lastreg").oninput = function() {
		checkLastname(this.value);
	}
	document.getElementById("idreg").oninput = function() {
		checkUsername(this.value);
	}
	document.getElementById("passwd").oninput = function() {
		checkPassword(this.value);
	}
	document.getElementById("mailreg").oninput = function() {
		checkEmail(this.value);
	}
	document.getElementById("cpassreg").oninput = function() {
		checkCPassword(this.value);
	}
}

function checkLastname(e) {
	var len = e.length;
	if (len < 1) {
		document.getElementById("lastnameerror").textContent = "Le nom de famille ne peut pas être vide";
		return (false);
	}
	document.getElementById("lastnameerror").textContent = "";
	return (true);
}

function checkFirstname(e) {
	var len = e.length;
	if (len < 1) {
		document.getElementById("firstnameerror").textContent = "Le prenom ne peut pas être vide";
		return (false);
	}
	document.getElementById("firstnameerror").textContent = "";
	return (true);
}

function checkPassword(e) {
	var test = /^(?=.*\d)(?=.*[a-z])(?=.*[A-Z])[0-9a-zA-Z]{8,}$/.test(e);
	if (!test) {
		document.getElementById("userpwderror").textContent = "Le mot de passe doit contenir au moins 8 caractères, un nombre, un caractère minuscule et un caractère majuscule.";
		return (false);
	}
	document.getElementById("userpwderror").textContent = "";
	return (true);
}

function checkUsername(e) {
	var test = /^[0-9a-zA-Z]{6,}$/.test(e);
	if (!test) {
		document.getElementById("usernameerror").textContent = "Le nom d'utilisateur doit contenir au moins 6 caractères alphanumériques.";
		return (false);
	}
	document.getElementById("usernameerror").textContent = "";
	return (true);
}

function formattedDate(d) {
	var month = String(d.getMonth() + 1);
	var day = String(d.getDate());
	var year = String(d.getFullYear());

	if (month.length < 2) month = '0' + month;
	if (day.length < 2) day = '0' + day;
	return `${day}/${month}/${year}`;
}

function checkDate(e) {
	var test = /^[0-9]{2}[\/]{1}[0-9]{2}[\/]{1}[0-9]{4}$/.test(e);
	if (!test && e.length > 0) {
		document.getElementById("birthdateerror").textContent = "La date doit être au format jj/mm/aaaa";
		return (false);
	}
	if (e.length > 0) {
		var parts = e.split("/");
		var date = new Date(+parts[2], parts[1] -1, +parts[0]);
		var cmpr = formattedDate(date);
		if (cmpr != e) {
			document.getElementById("birthdateerror").textContent = "La date est invalide";
			return (false);
		}
	}
	document.getElementById("birthdateerror").textContent = "";
	return (true);
}

function checkCPassword(e) {
	var userpwd = document.getElementById("passwd").value;
	if (e != userpwd) {
		document.getElementById("usercpwderror").textContent = "Les mots de passe ne correspondent pas";
		return (false);
	}
	document.getElementById("usercpwderror").textContent = "";
	return (true);
}

function checkEmail(e) {
	var test = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(e);
	if (!test) {
		document.getElementById("useremailerror").textContent = "L'addresse email est vide ou invalide";
		return (false);
	}
	document.getElementById("useremailerror").textContent = "";
	return (true);
}