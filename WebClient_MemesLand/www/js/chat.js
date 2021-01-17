// Masque la section du chat si l'utilisateur n'est pas connecté

function verifyLogged() {
	var un = getCookie('username');
	if (un == '') {
		$('#chatmain').html("<p>Vous devez être connecté pour continuer</p>");
	}
	else {
		getAllMsgs();
		var div = document.getElementById('chatwin');
		$('#chatwin').stop().animate({scrollTop: $('#chatwin')[0].scrollHeight}, 800);
	}
}

function msgToHTML(content) {
	return ('<p><strong>' + content.user + ' - </strong><i>' + content.date + ' - ' + content.time + '</i></p><p>' + content.msg + '</p><hr>');
}

function appendChat(content) {
	$(msgToHTML(content)).appendTo('#chatwin');
}

function updateMsgs(json) {
	$('#chatwin').html('');
	json.forEach(appendChat);
}

function getAllMsgs() {
	$.ajax({
		url : './htbin/chatget.py',
		type : 'POST',
		dataType : 'JSON',
		success : function(json_data, statut){
			updateMsgs(json_data);
		},
		error : function(result, statut, error){
			
		},
		complete : function(result, statut){
			
		}
	});
}

function sendMsg(msg) {
	$.ajax({
		url : './htbin/chatsend.py',
		type : 'POST',
		data : 'msg=' + msg,
		dataType : 'JSON',
		success : function(json_data, statut){
			if (json_data.num != 0) {
				alert(json_data.msg);
			}
		},
		error : function(result, statut, error){
			alert("Message sending failed");
		},
		complete : function(result, statut){
			
		}
	});
}

$(document).ready(function() {
	verifyLogged();
	$('input#chatsend').click(function() {
		var msg = $('textarea#chatinput').val();
		sendMsg(msg);
		getAllMsgs();
		$('textarea#chatinput').val('');
	});
});

window.setInterval(function() {
	getAllMsgs();
}, 10000);