import Image from "@theme/IdealImage";
import Layout from "@theme/Layout";
import Heading from "@theme/Heading";
import styles from "./gallery.module.css";
import { ReactNode } from "react";

const GalleryGrid = ({ images }) => {
  return (
    <div className={styles.galleryGrid}>
      {images.map((ideal, i) => {
        const largest = ideal.src.images.reduce(
          (max, img) => (img.width > max.width ? img : max),
          ideal.src.images[0]
        );
        return (
          <a
            href={largest.path}
            target="_blank"
            rel="noopener noreferrer"
            key={i}
          >
            <Image img={ideal} />
          </a>
        );
      })}
    </div>
  );
};

export default function Gallery(): ReactNode {
  return (
    <Layout title="Photo Gallery">
      <div className="container padding-vert--lg">
        <Heading as="h1">Photo Gallery</Heading>
        <Heading as="h2">Typeframe PX-88</Heading>
        <GalleryGrid
          images={[
            require("../../../docs/px-88/img/typeframe-px-88-01.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-02.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-03.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-04.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-05.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-06.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-07.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-08.jpg"),
          ]}
        />
        <Heading as="h2">Earlier Prototype with Alternate Keycaps</Heading>
        <GalleryGrid
          images={[
            require("../../../docs/px-88/img/typeframe-px-88-10.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-09.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-11.jpg"),
          ]}
        />
        <p>
          Did you build your own?{" "}
          <a href="mailto:jeff@typeframe.net">E-mail me</a> photos and I'll add
          them to the gallery!
        </p>
      </div>
    </Layout>
  );
}
