<template>
  <div class="hello">
    <h1>{{ msg }}</h1>
    <h2>Please Input</h2>
    <p>{{ message }}</p>
    <br>
    <textarea v-model="message" placeholder="add multiple lines"></textarea>
    <br>
    <h2>{{ return_value }}</h2>
  </div>
</template>

<script>
export default {
  name: 'HelloWorld',
  data () {
    return {
      msg: 'Welcome to Test Page!',
      return_value: ""
    }
  },

  created() {
    this.$axios.get("http://127.0.0.1:8000/test/").then(
      response => {
        console.log(response.data),
        this.return_value = response.data.message
      }
    )
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
p {
  white-space: pre-line;
}
</style>
