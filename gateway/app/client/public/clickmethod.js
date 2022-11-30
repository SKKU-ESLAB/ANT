function ProcessSearch(){
		const searchname = document.querySelector("#searchname").value;
		const xhr = new XMLHttpRequest();

		xhr.onreadystatechange = function() {
			console.log(this.readyState);
			if (this.readyState === 4) {
				const response = JSON.parse(this.responseText);
				$("#table_of_items tbody tr").remove();
				for (let i = 0; i < response.length; i++){
					insert_table(response[i]);
				}
			}

		}
		xhr.open('GET', '/search?category='+searchname, true);
		xhr.send();
	}
	function insert_table(process){
		var pid = process.pid;
		var command = process.command;
		var arguments = process.arguments[0];

		var thpid = document.createTextNode(pid);
		var tdcommand = document.createTextNode(command);
		var tdarguments = document.createTextNode(arguments);
		var tr = document.createElement("tr");
                var th = document.createElement("th");
                var td1 = document.createElement("td");
                var td2 = document.createElement("td");

		th.appendChild(thpid);
                td1.appendChild(tdcommand);
                td2.appendChild(tdarguments);

                tr.appendChild(th);
                tr.appendChild(td1);
                tr.appendChild(td2);
		var tbody = document.getElementsByTagName("tbody")[0];

		tbody.appendChild(tr);
	}
	function ProcessKill(){
		const killpid = document.querySelector("#killpid").value;
		const xhr = new XMLHttpRequest();

		console.log(this.readyState);
		xhr.onreadystatechange = function() {
			if (this.readyState == 4) {
				const reponse = JSON.parse(this.responseText);
				console.log(this.responseText);
			}
			for (let i = 0; i < response.length; i++){
				loadComments(response[i]);
			}
		}
	}
